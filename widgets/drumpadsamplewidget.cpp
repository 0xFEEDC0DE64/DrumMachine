#include "drumpadsamplewidget.h"
#include "ui_drumpadsamplewidget.h"

#include <QAbstractEventDispatcher>
#include <QSoundEffect>
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
}

DrumPadSampleWidget::DrumPadSampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::DrumPadSampleWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->dialSpeed, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setSpeed(float(value)/100.f); });
    connect(m_ui->dialVolume, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setVolume(float(value)/100.f); });

    connect(&m_player, &AudioPlayer::playingChanged, this, &DrumPadSampleWidget::updateStatus);

    connect(m_ui->pushButton, &QAbstractButton::pressed, this, [this](){ pressed(127); });
    connect(m_ui->pushButton, &QAbstractButton::released, this, &DrumPadSampleWidget::released);
    connect(m_ui->toolButtonLearn, &QAbstractButton::pressed, this, &DrumPadSampleWidget::learnPressed);

    updateStatus();
}

DrumPadSampleWidget::~DrumPadSampleWidget() = default;

void DrumPadSampleWidget::loadSettings(DrumMachineSettings &settings)
{
    m_ui->channelSpinBox->setValue(settings.drumpadChannel(m_padNr));
    m_ui->noteSpinBox->setValue(settings.drumpadNote(m_padNr));

    m_settings = &settings;
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

quint8 DrumPadSampleWidget::channel() const
{
    return m_ui->channelSpinBox->value();
}

void DrumPadSampleWidget::setChannel(quint8 channel)
{
    m_ui->channelSpinBox->setValue(channel);

    if (m_settings)
        m_settings->setDrumpadChannel(m_padNr, channel);
    else
        qWarning() << "no settings available";
}

quint8 DrumPadSampleWidget::note() const
{
    return m_ui->noteSpinBox->value();
}

void DrumPadSampleWidget::setNote(quint8 note)
{
    m_ui->noteSpinBox->setValue(note);

    if (m_settings)
        m_settings->setDrumpadNote(m_padNr, note);
    else
        qWarning() << "no settings available";
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

void DrumPadSampleWidget::pressed(quint8 velocity)
{
    Q_UNUSED(velocity)

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

void DrumPadSampleWidget::learn(quint8 channel, quint8 note)
{
    setChannel(channel);
    setNote(note);
    if (m_learning)
        learnPressed();
}

void DrumPadSampleWidget::unsendColor()
{
    m_sendColors = false;
    midi::MidiMessage midiMsg;

    midiMsg.channel = m_ui->channelSpinBox->value();
    midiMsg.cmd = midi::Command::NoteOn;
    midiMsg.flag = true;
    midiMsg.note = m_ui->noteSpinBox->value();
    midiMsg.velocity = 0;

    emit sendMidi(midiMsg);
}

void DrumPadSampleWidget::sendColor()
{
    m_sendColors = true;
    midi::MidiMessage midiMsg;

    midiMsg.channel = m_ui->channelSpinBox->value();
    midiMsg.cmd = midi::Command::NoteOn;
    midiMsg.flag = true;
    midiMsg.note = m_ui->noteSpinBox->value();

    if (m_file && m_file->color && m_player.buffer().isValid())
    {
        const auto &color = *m_file->color;
        if (color == "purple")
            midiMsg.velocity = m_player.playing() ? 43 : 18;
        else if (color == "red")
            midiMsg.velocity = m_player.playing() ? 3 : 1;
        else if (color == "yellow")
            midiMsg.velocity = m_player.playing() ? 58 : 33;
        else if (color == "green")
            midiMsg.velocity = m_player.playing() ? 56 : 16;
        else if (color == "blue")
            midiMsg.velocity = m_player.playing() ? 49 : 51;
        else
            goto noColor;
    }
    else
    {
        noColor:
        midiMsg.velocity = 0;
    }

    emit sendMidi(midiMsg);
}

void DrumPadSampleWidget::updateStatus()
{
    QPalette pal;


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
            pal.setColor(QPalette::Window, QColor{bright, dark, bright});
        else if (color == "red")
            pal.setColor(QPalette::Window, QColor{bright, dark, dark});
        else if (color == "yellow")
            pal.setColor(QPalette::Window, QColor{bright, bright, dark});
        else if (color == "green")
            pal.setColor(QPalette::Window, QColor{dark, bright, dark});
        else if (color == "blue")
            pal.setColor(QPalette::Window, QColor{dark, dark, bright});
        else
            qWarning() << "unknown color:" << color;
    }
    setPalette(pal);

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
    {
        emit startDecoding(m_reply);
    }
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

void DrumPadSampleWidget::learnPressed()
{
    auto palette = m_ui->toolButtonLearn->palette();

    if (m_learning)
    {
        palette.setColor(m_ui->toolButtonLearn->backgroundRole(), m_oldColor);
        palette.setBrush(m_ui->toolButtonLearn->backgroundRole(), m_oldBrush);
    }
    else
    {
        m_oldColor = palette.color(m_ui->toolButtonLearn->backgroundRole());
        m_oldBrush = palette.brush(m_ui->toolButtonLearn->backgroundRole());
        palette.setColor(m_ui->toolButtonLearn->backgroundRole(), Qt::red);
        palette.setBrush(m_ui->toolButtonLearn->backgroundRole(), Qt::red);
    }
    m_ui->toolButtonLearn->setPalette(palette);

    m_learning = !m_learning;
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