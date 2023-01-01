#include "loopstationwidget.h"
#include "ui_loopstationwidget.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include "audioformat.h"
#include "midicontainers.h"
#include "jsonconverters.h"
#include "loopstationjsonconverters.h"
#include "drummachinesettings.h"

LoopStationWidget::LoopStationWidget(QWidget *parent) :
    QSplitter{parent},
    m_ui{std::make_unique<Ui::LoopStationWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonUp, &QAbstractButton::pressed, this, &LoopStationWidget::selectPrevPreset);
    connect(m_ui->pushButtonDown, &QAbstractButton::pressed, this, &LoopStationWidget::selectNextPreset);
    connect(m_ui->pushButtonRefresh, &QAbstractButton::pressed, this, &LoopStationWidget::loadPresets);

    connect(m_ui->samplesWidget, &LoopStationSamplesWidget::sendMidi, this, &LoopStationWidget::sendMidi);

    m_presetsProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_presetsProxyModel.setSortRole(Qt::EditRole);
    m_presetsProxyModel.setSourceModel(&m_presetsModel);
    m_ui->presetsView->setModel(&m_presetsProxyModel);

    m_presetsProxyModel.setFilterKeyColumn(1);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, &m_presetsProxyModel, &QSortFilterProxyModel::setFilterFixedString);

    connect(m_ui->presetsView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &LoopStationWidget::currentRowChanged);
}

LoopStationWidget::~LoopStationWidget() = default;

void LoopStationWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_ui->samplesWidget->writeSamples(begin, end);
}

void LoopStationWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    loadPresets();
    m_ui->samplesWidget->injectNetworkAccessManager(networkAccessManager);
}

void LoopStationWidget::injectDecodingThread(QThread &thread)
{
    m_ui->samplesWidget->injectDecodingThread(thread);
}

void LoopStationWidget::loadSettings(DrumMachineSettings &settings)
{
    m_settings = &settings;

    m_ui->pushButtonUp->setLearnSetting(m_settings->loopstationPrevPreset());
    m_ui->pushButtonDown->setLearnSetting(m_settings->loopstationNextPreset());

    connect(m_ui->pushButtonUp, &MidiButton::learnSettingChanged, m_settings, &DrumMachineSettings::setLoopstationPrevPreset);
    connect(m_ui->pushButtonDown, &MidiButton::learnSettingChanged, m_settings, &DrumMachineSettings::setLoopstationNextPreset);

    m_ui->samplesWidget->loadSettings(settings);
}

void LoopStationWidget::unsendColors()
{
    const quint8 color = m_settings ? m_settings->colorOff() : quint8{0};

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonUp->learnSetting().channel,
        .cmd = m_ui->pushButtonUp->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonUp->learnSetting().note,
        .velocity = color
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonDown->learnSetting().channel,
        .cmd = m_ui->pushButtonDown->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonDown->learnSetting().note,
        .velocity = color
    });

    m_ui->samplesWidget->unsendColors();
}

void LoopStationWidget::sendColors()
{
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonUp->learnSetting().channel,
        .cmd = m_ui->pushButtonUp->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonUp->learnSetting().note,
        .velocity = m_settings ? m_settings->loopstationColorPrevPreset() : quint8{127}
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonDown->learnSetting().channel,
        .cmd = m_ui->pushButtonDown->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonDown->learnSetting().note,
        .velocity = m_settings ? m_settings->loopstationColorNextPreset() : quint8{127}
    });

    m_ui->samplesWidget->sendColors();
}

void LoopStationWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonUp->midiReceived(message);
    m_ui->pushButtonDown->midiReceived(message);

    m_ui->samplesWidget->midiReceived(message);
}

void LoopStationWidget::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
    {
        Q_ASSERT(m_settings);
        if (m_settings)
            m_settings->setLoopstationLastPresetId(QString{});
        else
            qWarning() << "no settings available";
        return;
    }

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    Q_ASSERT(m_settings);
    if (m_settings)
        m_settings->setLoopstationLastPresetId(preset.id ? *preset.id : QString{});
    else
        qWarning() << "no settings available";

    m_ui->presetDetailWidget->setPreset(preset);
    m_ui->samplesWidget->setPreset(preset);
}

void LoopStationWidget::loadPresets()
{
    if (!m_networkAccessManager)
    {
        qWarning() << "no network access manager available";
        return;
    }

    m_ui->pushButtonRefresh->setEnabled(false);

    QNetworkRequest request{QUrl{"https://brunner.ninja/komposthaufen/groovepad/presets_config_v2.json"}};
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);

    m_reply = std::unique_ptr<QNetworkReply>(m_networkAccessManager->get(request));
    connect(m_reply.get(), &QNetworkReply::finished, this, &LoopStationWidget::requestFinished);
}

void LoopStationWidget::requestFinished()
{
    if (!m_reply)
    {
        qWarning() << "no valid reply";
        return;
    }

    m_ui->pushButtonRefresh->setEnabled(true);

    auto reply = std::move(m_reply);
    try
    {
        if (reply->error() != QNetworkReply::NoError)
            throw std::runtime_error{QString{"request failed: %0"}.arg(reply->errorString()).toStdString()};

        auto result = json_converters::loopstation::parsePresetsConfig(json_converters::loadJson(reply->readAll()));

        if (!result.presets)
            throw std::runtime_error("presets missing in response");

        m_presetsModel.setPresets(std::move(*std::move(result).presets));

        if (m_settings)
        {
            if (const auto &lastPresetId = m_settings->loopstationLastPresetId(); !lastPresetId.isEmpty())
            {
                if (const auto &index = m_presetsModel.findPresetById(lastPresetId); index.isValid())
                    selectIndex(m_presetsProxyModel.mapFromSource(index));
                else
                {
                    qWarning() << "invalid last preset id" << lastPresetId;
                    goto noLastId;
                }
            }
            else
                goto noLastId;
        }
        else
        {
noLastId:
            selectFirstPreset();
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, tr("Could not load presets!"), tr("Could not load presets!") + "\n\n" + QString::fromStdString(e.what()));
    }
}

void LoopStationWidget::selectFirstPreset()
{
    if (!m_presetsProxyModel.rowCount())
        return;

    selectIndex(m_presetsProxyModel.index(0, 0));
}

void LoopStationWidget::selectPrevPreset()
{
    if (!m_presetsProxyModel.rowCount())
        return;

    const auto index = m_ui->presetsView->selectionModel()->currentIndex();
    if (!index.isValid())
    {
        qWarning() << "invalid index";
        return;
    }

    if (index.row() > 0)
        selectIndex(m_presetsProxyModel.index(index.row() - 1, 0));
}

void LoopStationWidget::selectNextPreset()
{
    if (!m_presetsProxyModel.rowCount())
        return;

    const auto index = m_ui->presetsView->selectionModel()->currentIndex();
    if (!index.isValid())
    {
        qWarning() << "invalid index";
        return;
    }

    if (index.row() + 1 < m_presetsProxyModel.rowCount())
        selectIndex(m_presetsProxyModel.index(index.row() + 1, 0));
}

void LoopStationWidget::selectIndex(const QModelIndex &index)
{
    if (!index.isValid())
    {
        qWarning() << "invalid index";
        return;
    }

    m_ui->presetsView->scrollTo(index);
//    m_ui->presetsView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    m_ui->presetsView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    currentRowChanged(index);
}
