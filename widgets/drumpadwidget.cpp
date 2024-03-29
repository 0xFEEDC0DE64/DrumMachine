#include "drumpadwidget.h"
#include "ui_drumpadwidget.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include "audioformat.h"
#include "midicontainers.h"
#include "jsonconverters.h"
#include "drumpadjsonconverters.h"
#include "drummachinesettings.h"

DrumPadWidget::DrumPadWidget(QWidget *parent) :
    QSplitter{parent},
    m_ui{std::make_unique<Ui::DrumPadWidget>()},
    m_presetsModel{this},
    m_presetsProxyModel{this},
    m_filesModel{this}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonUp, &QAbstractButton::pressed, this, &DrumPadWidget::selectPrevPreset);
    connect(m_ui->pushButtonDown, &QAbstractButton::pressed, this, &DrumPadWidget::selectNextPreset);
    connect(m_ui->pushButtonRefresh, &QAbstractButton::pressed, this, &DrumPadWidget::loadPresets);

    connect(m_ui->sequencerWidget, &DrumPadSequencerWidget::sendMidi, this, &DrumPadWidget::sendMidi);
    connect(m_ui->samplesWidget, &DrumPadSamplesWidget::sendMidi, this, &DrumPadWidget::sendMidi);

    connect(m_ui->sequencerWidget, &DrumPadSequencerWidget::triggerSample, m_ui->samplesWidget, &DrumPadSamplesWidget::sequencerTriggerSample);

    m_presetsProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_presetsProxyModel.setSortRole(Qt::EditRole);
    m_presetsProxyModel.setSourceModel(&m_presetsModel);
    m_ui->presetsView->setModel(&m_presetsProxyModel);

    m_presetsProxyModel.setFilterKeyColumn(1);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, &m_presetsProxyModel, &QSortFilterProxyModel::setFilterFixedString);

    m_ui->filesView->setModel(&m_filesModel);

    connect(m_ui->presetsView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &DrumPadWidget::currentRowChanged);
}

DrumPadWidget::~DrumPadWidget() = default;

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
    m_settings = &settings;

    m_ui->pushButtonUp->setLearnSetting(m_settings->drumpadPrevPreset());
    m_ui->pushButtonDown->setLearnSetting(m_settings->drumpadNextPreset());

    connect(m_ui->pushButtonUp, &MidiButton::learnSettingChanged, m_settings, &DrumMachineSettings::setDrumpadPrevPreset);
    connect(m_ui->pushButtonDown, &MidiButton::learnSettingChanged, m_settings, &DrumMachineSettings::setDrumpadNextPreset);

    m_ui->sequencerWidget->loadSettings(settings);
    m_ui->samplesWidget->loadSettings(settings);
}

void DrumPadWidget::unsendColors()
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

    m_ui->sequencerWidget->unsendColors();
    m_ui->samplesWidget->unsendColors();
}

void DrumPadWidget::sendColors()
{
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonUp->learnSetting().channel,
        .cmd = m_ui->pushButtonUp->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonUp->learnSetting().note,
        .velocity = m_settings ? m_settings->drumpadColorPrevPreset() : quint8{127}
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonDown->learnSetting().channel,
        .cmd = m_ui->pushButtonDown->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonDown->learnSetting().note,
        .velocity = m_settings ? m_settings->drumpadColorNextPreset() : quint8{127}
    });

    m_ui->sequencerWidget->sendColors();
    m_ui->samplesWidget->sendColors();
}

void DrumPadWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonUp->midiReceived(message);
    m_ui->pushButtonDown->midiReceived(message);

    m_ui->sequencerWidget->midiReceived(message);
    m_ui->samplesWidget->midiReceived(message);
}

void DrumPadWidget::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
    {
        Q_ASSERT(m_settings);
        if (m_settings)
            m_settings->setDrumpadLastPresetId(QString{});
        else
            qWarning() << "no settings available";
        return;
    }

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    Q_ASSERT(m_settings);
    if (m_settings)
        m_settings->setDrumpadLastPresetId(preset.id ? *preset.id : QString{});
    else
        qWarning() << "no settings available";

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

        auto result = json_converters::drumpad::parsePresetsConfig(json_converters::loadJson(reply->readAll()));

        if (!result.presets)
            throw std::runtime_error("presets missing in response");

        m_presetsModel.setPresets(std::move(*std::move(result).presets));

        if (m_settings)
        {
            if (const auto &lastPresetId = m_settings->drumpadLastPresetId(); !lastPresetId.isEmpty())
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

void DrumPadWidget::selectFirstPreset()
{
    if (!m_presetsProxyModel.rowCount())
        return;

    selectIndex(m_presetsProxyModel.index(0, 0));
}

void DrumPadWidget::selectPrevPreset()
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

void DrumPadWidget::selectNextPreset()
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

void DrumPadWidget::selectIndex(const QModelIndex &index)
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
