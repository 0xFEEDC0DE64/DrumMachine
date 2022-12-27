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
    Q_UNUSED(begin)
    Q_UNUSED(end)
}

void LoopStationWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    loadPresets();
}

void LoopStationWidget::injectDecodingThread(QThread &thread)
{
    Q_UNUSED(thread)
}

void LoopStationWidget::loadSettings(DrumMachineSettings &settings)
{
    m_settings = &settings;
}

void LoopStationWidget::unsendColors()
{

}

void LoopStationWidget::sendColors()
{

}

void LoopStationWidget::midiReceived(const midi::MidiMessage &message)
{
    Q_UNUSED(message);
}

void LoopStationWidget::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
    {
        if (m_settings)
            m_settings->setLoopstationLastPresetId(QString{});
        else
            qWarning() << "no settings available";
        return;
    }

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    if (m_settings)
        m_settings->setLoopstationLastPresetId(preset.id ? *preset.id : QString{});
    else
        qWarning() << "no settings available";
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

    auto reply = std::move(m_reply);
    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this, tr("Could not load presets!"), tr("Could not load presets!") + "\n\n" + reply->errorString());
        return;
    }

    try
    {
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
        QMessageBox::warning(this, tr("error"), tr("error") + "\n\n" + QString::fromStdString(e.what()));
    }
}

void LoopStationWidget::selectFirstPreset()
{

}

void LoopStationWidget::selectPrevPreset()
{

}

void LoopStationWidget::selectNextPreset()
{

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
