#include "loopstationsamplewidget.h"
#include "ui_loopstationsamplewidget.h"

#include <QAbstractEventDispatcher>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMetaEnum>

#include "audioformat.h"
#include "audiodecoder.h"
#include "drummachinesettings.h"
#include "midicontainers.h"

LoopStationSampleWidget::LoopStationSampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::LoopStationSampleWidget>()}
{
    m_ui->setupUi(this);

    connect(&m_player, &AudioPlayer::playingChanged, this, &LoopStationSampleWidget::updateStatus);

    connect(m_ui->pushButtonPlay, &QAbstractButton::pressed, this, &LoopStationSampleWidget::pressed);

    updateStatus();
}

LoopStationSampleWidget::~LoopStationSampleWidget() = default;

void LoopStationSampleWidget::loadSettings(DrumMachineSettings &settings)
{
    m_settings = &settings;

    m_ui->pushButtonPlay->setLearnSetting(m_settings->loopstationSample(m_padNr));

    connect(m_ui->pushButtonPlay, &MidiButton::learnSettingChanged, this, [this](const MidiLearnSetting &learnSetting){
        Q_ASSERT(m_settings);
        if (m_settings)
            m_settings->setLoopstationSample(m_padNr, learnSetting);
        else
            qWarning() << "no settings available";
    });
}

void LoopStationSampleWidget::setSample(const QString &presetId, const QString &filename, const QString &type)
{
    m_presetId = presetId;
    m_filename = filename;

    m_player.setBuffer({});

    startRequest();

    m_ui->labelFilename->setText(filename);
    m_ui->labelType->setText(type);
}

void LoopStationSampleWidget::pressed()
{
    m_player.restart();
}

void LoopStationSampleWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    if (!m_presetId.isEmpty() && !m_filename.isEmpty())
        startRequest();
}

void LoopStationSampleWidget::injectDecodingThread(QThread &thread)
{
    QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(&thread), [this](){
        m_decoder = std::make_unique<AudioDecoder>();
        connect(this, &LoopStationSampleWidget::startDecoding, m_decoder.get(), &AudioDecoder::startDecodingDevice);
        connect(m_decoder.get(), &AudioDecoder::decodingFinished, this, &LoopStationSampleWidget::decodingFinished);
        if (m_reply && m_reply->isFinished() && m_reply->error() == QNetworkReply::NoError)
            m_decoder->startDecodingDevice(m_reply);
    });
}

void LoopStationSampleWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_player.writeSamples(begin, end);
}

void LoopStationSampleWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonPlay->midiReceived(message);
}

void LoopStationSampleWidget::unsendColor()
{
    m_sendColors = false;

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlay->learnSetting().channel,
        .cmd = m_ui->pushButtonPlay->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlay->learnSetting().note,
        .velocity = 0
    });
}

void LoopStationSampleWidget::sendColor()
{
    m_sendColors = true;

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlay->learnSetting().channel,
        .cmd = m_ui->pushButtonPlay->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlay->learnSetting().note,
        .velocity = uint8_t(m_padNr+1)
    });
}

void LoopStationSampleWidget::updateStatus()
{
    if (m_sendColors)
        sendColor();

    if (m_reply)
    {
        if (!m_reply->isFinished())
            m_ui->labelStatus->setText(tr("Downloading..."));
        else if (m_reply->error() != QNetworkReply::NoError)
            m_ui->labelStatus->setText(QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(m_reply->error()));
        else
        {
            if (!m_decoder)
                m_ui->labelStatus->setText(tr("Waiting for decoder thread..."));
            else
                m_ui->labelStatus->setText(tr("Decoding"));
        }
    }
    else
        m_ui->labelStatus->setText(m_player.playing() ? tr("Playing") : tr("Ready"));
}

void LoopStationSampleWidget::requestFinished()
{
    if (m_reply->error() == QNetworkReply::NoError)
        emit startDecoding(m_reply);
    updateStatus();
}

void LoopStationSampleWidget::decodingFinished(const QAudioBuffer &buffer)
{
    m_reply = nullptr;
    m_player.setBuffer(buffer);
    updateStatus();
}

void LoopStationSampleWidget::startRequest()
{
    if (m_networkAccessManager && !m_presetId.isEmpty() && !m_filename.isEmpty())
    {
        QNetworkRequest request{QUrl{QString{"https://brunner.ninja/komposthaufen/groovepad/presets/extracted/%0/%1"}.arg(m_presetId, m_filename)}};
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);
        m_reply = std::shared_ptr<QNetworkReply>{m_networkAccessManager->get(request)};
        connect(m_reply.get(), &QNetworkReply::finished, this, &LoopStationSampleWidget::requestFinished);
    }

    updateStatus();
}
