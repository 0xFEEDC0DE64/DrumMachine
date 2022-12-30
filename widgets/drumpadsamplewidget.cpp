#include "drumpadsamplewidget.h"
#include "ui_drumpadsamplewidget.h"

#include <QAbstractEventDispatcher>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMetaEnum>

#include "audioformat.h"
#include "audiodecoder.h"
#include "drummachinesettings.h"
#include "midicontainers.h"

namespace {
QString toString(QString value) { return value; }
QString toString(int value) { return QString::number(value); }
QString toString(bool value) { return value?"true":"false"; }
} // namespace

DrumPadSampleWidget::DrumPadSampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::DrumPadSampleWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->dialSpeed, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setSpeed(float(value)/100.f); });
    connect(m_ui->dialVolume, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setVolume(float(value)/100.f); });

    connect(&m_player, &AudioPlayer::playingChanged, this, &DrumPadSampleWidget::updateStatus);

    connect(m_ui->pushButtonPlay, &QAbstractButton::pressed, this, &DrumPadSampleWidget::pressed);
    connect(m_ui->pushButtonPlay, &QAbstractButton::released, this, &DrumPadSampleWidget::released);

    updateStatus();
}

DrumPadSampleWidget::~DrumPadSampleWidget() = default;

void DrumPadSampleWidget::loadSettings(DrumMachineSettings &settings)
{
    m_settings = &settings;

    m_ui->pushButtonPlay->setLearnSetting(m_settings->drumpadSample(m_padNr));

    connect(m_ui->pushButtonPlay, &MidiButton::learnSettingChanged, this, [this](const MidiLearnSetting &learnSetting){
        Q_ASSERT(m_settings);
        if (m_settings)
            m_settings->setDrumpadSample(m_padNr, learnSetting);
        else
            qWarning() << "no settings available";
    });
}

void DrumPadSampleWidget::setFile(const QString &presetId, const drumpad_presets::File &file)
{
    m_presetId = presetId;
    m_file = file;

    m_player.setBuffer({});

    startRequest();

    const auto setupLabel = [&](const auto &value, QLabel *label){
        QString text;
        QFont font;
        QPalette pal;

        if (value)
            text = toString(*value);
        else
        {
            text = tr("(null)");
            font.setItalic(true);
            pal.setColor(label->foregroundRole(), Qt::gray);
        }

        label->setText(text);
        label->setFont(font);
        label->setPalette(pal);
    };

    setupLabel(file.stopOnRelease, m_ui->stopOnReleaseLabel);
    setupLabel(file.looped, m_ui->loopedLabel);
    setupLabel(file.choke, m_ui->chokeLabel);
}

int DrumPadSampleWidget::speed() const
{
    return m_ui->dialSpeed->value();
}

void DrumPadSampleWidget::setSpeed(int speed)
{
    m_ui->dialSpeed->setValue(speed);
}

int DrumPadSampleWidget::volume() const
{
    return m_ui->dialVolume->value();
}

void DrumPadSampleWidget::setVolume(int volume)
{
    m_ui->dialVolume->setValue(volume);
}

std::optional<int> DrumPadSampleWidget::choke() const
{
    if (!m_file)
        return {};
    return m_file->choke;
}

void DrumPadSampleWidget::pressed()
{
    m_player.restart();

    if (m_file && m_file->choke && *m_file->choke)
        emit chokeTriggered(*m_file->choke);
}

void DrumPadSampleWidget::released()
{
}

void DrumPadSampleWidget::forceStop()
{
    m_player.setPlaying(false);
}

void DrumPadSampleWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    if (m_file)
        startRequest();
}

void DrumPadSampleWidget::injectDecodingThread(QThread &thread)
{
    QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(&thread), [this](){
        m_decoder = std::make_unique<AudioDecoder>();
        connect(this, &DrumPadSampleWidget::startDecoding, m_decoder.get(), &AudioDecoder::startDecodingDevice);
        connect(m_decoder.get(), &AudioDecoder::decodingFinished, this, &DrumPadSampleWidget::decodingFinished);
        if (m_reply && m_reply->isFinished() && m_reply->error() == QNetworkReply::NoError)
            m_decoder->startDecodingDevice(m_reply);
    });
}

void DrumPadSampleWidget::writeSamples(frame_t *begin, frame_t *end)
{
    m_player.writeSamples(begin, end);
}

void DrumPadSampleWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonPlay->midiReceived(message);
}

void DrumPadSampleWidget::unsendColor()
{
    m_sendColors = false;

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlay->learnSetting().channel,
        .cmd = m_ui->pushButtonPlay->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlay->learnSetting().note,
        .velocity = 0
    });
    m_lastMidiColor = 0;
}

void DrumPadSampleWidget::sendColor()
{
    m_sendColors = true;

    uint8_t newColor;

    if (m_file && m_file->color && m_player.buffer().isValid())
    {
        const auto &color = *m_file->color;
        if (color == "purple")
            newColor = m_player.playing() ? 43 : 18;
        else if (color == "red")
            newColor = m_player.playing() ? 3 : 1;
        else if (color == "yellow")
            newColor = m_player.playing() ? 58 : 33;
        else if (color == "green")
            newColor = m_player.playing() ? 56 : 16;
        else if (color == "blue")
            newColor = m_player.playing() ? 49 : 51;
        else
            goto noColor;
    }
    else
    {
noColor:
        newColor = 0;
    }

    if (newColor != m_lastMidiColor)
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

void DrumPadSampleWidget::updateStatus()
{
    QColor newColor;
    if (m_file && m_file->color && m_player.buffer().isValid())
    {
        const auto bright = m_player.playing() ? 255 : 155;
        const auto dark = m_player.playing() ?
#if !defined(Q_OS_WIN)
        80 : 0
#else
        180 : 80
#endif
;

        const auto &color = *m_file->color;
        if (color == "purple")
            newColor = QColor{bright, dark, bright};
        else if (color == "red")
            newColor = QColor{bright, dark, dark};
        else if (color == "yellow")
            newColor = QColor{bright, bright, dark};
        else if (color == "green")
            newColor = QColor{dark, bright, dark};
        else if (color == "blue")
            newColor = QColor{dark, dark, bright};
        else
        {
            qWarning() << "unknown color:" << color;
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
        sendColor();

    if (m_reply)
    {
        if (!m_reply->isFinished())
            m_ui->statusLabel->setText(tr("Downloading..."));
        else if (m_reply->error() != QNetworkReply::NoError)
            m_ui->statusLabel->setText(QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(m_reply->error()));
        else
        {
            if (!m_decoder)
                m_ui->statusLabel->setText(tr("Waiting for decoder thread..."));
            else
                m_ui->statusLabel->setText(tr("Decoding"));
        }
    }
    else
        m_ui->statusLabel->setText(m_player.playing() ? tr("Playing") : tr("Ready"));
}

void DrumPadSampleWidget::requestFinished()
{
    if (m_reply->error() == QNetworkReply::NoError)
        emit startDecoding(m_reply);
    updateStatus();
}

void DrumPadSampleWidget::decodingFinished(const QAudioBuffer &buffer)
{
    m_reply = nullptr;
    m_player.setBuffer(buffer);
    setSpeed(100);
    setVolume(100);
    updateStatus();
}

void DrumPadSampleWidget::startRequest()
{
    if (m_networkAccessManager && m_file->filename)
    {
        QNetworkRequest request{QUrl{QString{"https://brunner.ninja/komposthaufen/dpm/presets/extracted/%0/%1"}.arg(m_presetId, *m_file->filename)}};
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);
        m_reply = std::shared_ptr<QNetworkReply>{m_networkAccessManager->get(request)};
        connect(m_reply.get(), &QNetworkReply::finished, this, &DrumPadSampleWidget::requestFinished);
    }

    updateStatus();
}
