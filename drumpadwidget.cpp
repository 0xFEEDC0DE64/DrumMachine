#include "drumpadwidget.h"
#include "ui_drumpadwidget.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include "jsonconverters.h"

DrumPadWidget::DrumPadWidget(QWidget *parent) :
    QSplitter{parent},
    m_ui{std::make_unique<Ui::DrumPadWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->samplesWidget, &SamplesWidget::sendMidi, this, &DrumPadWidget::sendMidi);

    connect(m_ui->sequencerWidget, &SequencerWidget::triggerSample, m_ui->samplesWidget, &SamplesWidget::sequencerTriggerSample);

    m_presetsProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_presetsProxyModel.setSortRole(Qt::EditRole);
    m_presetsProxyModel.setSourceModel(&m_presetsModel);
    m_ui->presetsView->setModel(&m_presetsProxyModel);

    m_presetsProxyModel.setFilterKeyColumn(1);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, &m_presetsProxyModel, &QSortFilterProxyModel::setFilterFixedString);

    m_ui->filesView->setModel(&m_filesModel);

    connect(m_ui->presetsView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &DrumPadWidget::currentRowChanged);

    connect(m_ui->pushButtonRefresh, &QAbstractButton::pressed, this, &DrumPadWidget::loadPresets);
}

DrumPadWidget::~DrumPadWidget() = default;

void DrumPadWidget::selectFirstPreset()
{
    if (!m_presetsProxyModel.rowCount())
        return;

    const auto index = m_presetsProxyModel.index(0, 0);
    if (index.isValid())
    {
        m_ui->presetsView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        currentRowChanged(index);
    }
}

void DrumPadWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_ui->samplesWidget->writeSamples(begin, end);
}

void DrumPadWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    loadPresets();
    m_ui->samplesWidget->injectNetworkAccessManager(networkAccessManager);
}

void DrumPadWidget::injectDecodingThread(QThread &thread)
{
    m_ui->samplesWidget->injectDecodingThread(thread);
}

void DrumPadWidget::loadSettings(DrumMachineSettings &settings)
{
    m_ui->samplesWidget->loadSettings(settings);
}

void DrumPadWidget::unsendColors()
{
    m_ui->samplesWidget->unsendColors();
}

void DrumPadWidget::sendColors()
{
    m_ui->samplesWidget->sendColors();
}

void DrumPadWidget::messageReceived(const midi::MidiMessage &message)
{
    m_ui->samplesWidget->messageReceived(message);
}

void DrumPadWidget::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
        return;

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    m_ui->presetDetailWidget->setPreset(preset);
    m_filesModel.setPreset(preset);
    m_ui->sequencerWidget->setPreset(preset);
    m_ui->samplesWidget->setPreset(preset);
}

void DrumPadWidget::loadPresets()
{
    if (!m_networkAccessManager)
    {
        qWarning() << "no network access manager available";
        return;
    }

    m_ui->pushButtonRefresh->setEnabled(false);

    QNetworkRequest request{QUrl{"https://brunner.ninja/komposthaufen/dpm/presets_config_v12.json"}};
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);

    m_reply = std::unique_ptr<QNetworkReply>(m_networkAccessManager->get(request));
    connect(m_reply.get(), &QNetworkReply::finished, this, &DrumPadWidget::requestFinished);
}

void DrumPadWidget::requestFinished()
{
    qDebug() << "called";

    if (!m_reply)
    {
        qWarning() << "no valid reply";
        return;
    }

    auto reply = std::move(m_reply);
    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this, tr("Could not load presets!"), tr("Could not load presets!") + "\n\n" + reply->errorString());
        return;
    }

    try
    {
        auto result = json_converters::parsePresetsConfig(json_converters::loadJson(reply->readAll()));

        if (!result.presets)
            throw std::runtime_error("presets missing in response");

        m_presetsModel.setPresets(std::move(*std::move(result).presets));

        selectFirstPreset();
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, tr("error"), tr("error") + "\n\n" + QString::fromStdString(e.what()));
    }
}
