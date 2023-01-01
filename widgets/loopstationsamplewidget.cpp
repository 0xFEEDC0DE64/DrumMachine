#include "loopstationsamplewidget.h"
#include "ui_loopstationsamplewidget.h"

#include <QAbstractEventDispatcher>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMetaEnum>
#include <QPalette>

#include "audioformat.h"
#include "audiodecoder.h"
#include "drummachinesettings.h"
#include "midicontainers.h"

LoopStationSampleWidget::LoopStationSampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::LoopStationSampleWidget>()},
    m_player{this}
{
    m_ui->setupUi(this);

    connect(&m_player, &AudioPlayer::playingChanged, this, &LoopStationSampleWidget::updateStatus);

    connect(m_ui->pushButtonPlay, &QAbstractButton::toggled, this, [this](bool toggled){
        emit loopEnabledChanged(toggled, m_category);
    });

    updateStatus();
}

LoopStationSampleWidget::~LoopStationSampleWidget() = default;

void LoopStationSampleWidget::setCategory(quint8 category)
{
    m_category = category;
    m_ui->labelCategory->setText(QString::number(category));
    updateStatus();
}

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

    const quint8 color = m_settings ? m_settings->colorOff() : quint8{0};

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlay->learnSetting().channel,
        .cmd = m_ui->pushButtonPlay->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlay->learnSetting().note,
        .velocity = color
    });

    m_lastMidiColor = color;
}

void LoopStationSampleWidget::sendColor(bool force)
{
    m_sendColors = true;

    quint8 newColor;

    if (false) // testing colors on launchpad mk2
        newColor = m_padNr + 48;
    else
    {
        if (m_player.buffer().isValid())
        {
            if (m_category == 0)
                newColor = m_player.playing() ? 44 : 47; //dunkelblue
            else if (m_category == 1)
                newColor = m_player.playing() ? 16 : 19; // green
            else if (m_category == 2)
                newColor = m_player.playing() ? 48 : 51; // violet
            else if (m_category == 3)
                newColor = m_player.playing() ? 36 : 39; // hellblue
            else if (m_category == 4)
                newColor = m_player.playing() ? 8  : 11; // orange
            else if (m_category == 5)
                newColor = m_player.playing() ? 52 : 55; // pink
            else
                newColor = m_player.playing() ? 4 : 7; // red
        }
        else
        {
            newColor = 0;
        }
    }

    if (force || newColor != m_lastMidiColor)
    {
        emit sendMidi(midi::MidiMessage {
            .channel = m_ui->pushButtonPlay->learnSetting().channel,
            .cmd = m_ui->pushButtonPlay->learnSetting().cmd,
            .flag = true,
            .note = m_ui->pushButtonPlay->learnSetting().note,
            .velocity = newColor
        });
        m_lastMidiColor = newColor;
    }
}

bool LoopStationSampleWidget::loopEnabled() const
{
    return m_ui->pushButtonPlay->isChecked();
}

void LoopStationSampleWidget::timeout()
{
    if (m_ui->pushButtonPlay->isChecked())
    {
        // if is not playing or position > 80%
        if (!m_player.playing() || m_player.position() >= m_player.buffer().frameCount() * 8 / 10)
            m_player.restart();
    }
    else
    {
        // if is playing and position < 60%
        if (m_player.playing() && m_player.position() < m_player.buffer().frameCount() * 6 / 10)
            m_player.stop();
    }
}

void LoopStationSampleWidget::setLoopEnabled(bool enabled)
{
    m_ui->pushButtonPlay->setChecked(enabled);
}

void LoopStationSampleWidget::stop()
{
    m_player.stop();
}

void LoopStationSampleWidget::updateStatus()
{
    QColor newColor;

    if (m_player.buffer().isValid())
    {
        const auto bright = m_player.playing() ? 255 : 155;
        const auto dark = m_player.playing() ?
#if !defined(Q_OS_WIN)
        80 : 0
#else
        180 : 80
#endif
;

        if (m_category == 0)
            newColor = QColor{bright, dark, bright};
        else if (m_category == 1)
            newColor = QColor{bright, dark, dark};
        else if (m_category == 2)
            newColor = QColor{bright, bright, dark};
        else if (m_category == 3)
            newColor = QColor{dark, bright, dark};
        else if (m_category == 4)
            newColor = QColor{dark, dark, bright};
        else if (m_category == 5)
            newColor = QColor{dark, bright, bright};
        else
        {
            qWarning() << "unknown category:" << m_category;
            newColor = QColor{dark, dark, dark};
        }
    }

    if (newColor.isValid() && (!m_lastColor.isValid() || newColor != m_lastColor))
    {
        QPalette pal;
        pal.setColor(QPalette::Window, newColor);
        m_lastColor = newColor;
        setPalette(pal);
    }

    if (m_sendColors)
        sendColor(false);

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
