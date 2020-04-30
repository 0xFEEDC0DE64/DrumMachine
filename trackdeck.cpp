#include "trackdeck.h"
#include "ui_trackdeck.h"

#include <QAbstractEventDispatcher>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTextStream>
#include <QUrl>
#include <QFileInfo>
#include <QButtonGroup>
#include <QDebug>

#include "audiodecoder.h"

TrackDeck::TrackDeck(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::TrackDeck>()}
{
    m_ui->setupUi(this);
    m_ui->progressBar->hide();

    m_loopGroup.addButton(m_ui->pushButtonLoopOff, -1);
    m_loopGroup.addButton(m_ui->pushButtonLoop1, 1);
    m_loopGroup.addButton(m_ui->pushButtonLoop2, 2);
    m_loopGroup.addButton(m_ui->pushButtonLoop4, 4);
    m_loopGroup.addButton(m_ui->pushButtonLoop8, 8);
    connect(&m_loopGroup, qOverload<int>(&QButtonGroup::buttonClicked), [&player=m_player,&bpmInput=*m_ui->doubleSpinBoxBpm](int id){
        const auto position = player.position();

        const auto bpm = bpmInput.value();
        const auto beatsPerSecond = bpm / 60.;
        const auto framesPerBeat = frameRate / beatsPerSecond;

        switch (id)
        {
        case 1: player.setLoop(std::make_pair(position, position + (framesPerBeat/4))); break;
        case 2: player.setLoop(std::make_pair(position, position + (framesPerBeat/2))); break;
        case 4: player.setLoop(std::make_pair(position, position + framesPerBeat)); break;
        case 8: player.setLoop(std::make_pair(position, position + (framesPerBeat*2))); break;
        default: player.setLoop({});
        }
    });

    connect(m_ui->pushButtonBpm, &QAbstractButton::pressed, this, &TrackDeck::bpmTap);

    connect(m_ui->pushButtonPlay, &QAbstractButton::pressed, &m_player, &AudioPlayer::togglePlaying);
    connect(m_ui->pushButtonStop, &QAbstractButton::pressed, &m_player, &AudioPlayer::stop);

    connect(m_ui->sliderZoom, &QAbstractSlider::valueChanged, m_ui->scratchWidget, &ScratchWidget::setBeatWidth);
    connect(m_ui->sliderSpeed, &QAbstractSlider::valueChanged, this, &TrackDeck::speedChanged);
    connect(m_ui->sliderVolume, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setVolume(float(value)/100.f); });

    connect(m_ui->previewWidget, &PreviewWidget::positionSelected, &m_player, &AudioPlayer::setPosition);
    connect(&m_player, &AudioPlayer::positionChanged, m_ui->previewWidget, &PreviewWidget::setPosition);
    connect(m_ui->scratchWidget, &ScratchWidget::scratchBegin, this, &TrackDeck::scratchBegin);
    connect(m_ui->scratchWidget, &ScratchWidget::scratchEnd, this, &TrackDeck::scratchEnd);
    connect(&m_player, &AudioPlayer::positionChanged, m_ui->scratchWidget, &ScratchWidget::setPosition);
    connect(&m_player, &AudioPlayer::playingChanged, this, &TrackDeck::updatePlayButtonText);

    connect(m_ui->sliderSpeed, &QAbstractSlider::valueChanged, this, &TrackDeck::updatePlaybackBpm);
    connect(m_ui->doubleSpinBoxBpm, &QDoubleSpinBox::valueChanged, this, &TrackDeck::updatePlaybackBpm);

    connect(&m_timer, &QTimer::timeout, this, &TrackDeck::timeout);
    m_timer.setSingleShot(true);
    m_timer.setInterval(1000);

    updatePlaybackBpm();
}

TrackDeck::~TrackDeck() = default;

void TrackDeck::loadTrack(const QString &filename)
{
    m_filename = filename;
    emit startDecoding(filename);
    m_ui->progressBar->show();
    m_ui->progressBar->setValue(0);
}

void TrackDeck::injectDecodingThread(QThread &thread)
{
    QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(&thread), [this](){
        m_decoder = std::make_unique<AudioDecoder>();
        connect(this, &TrackDeck::startDecoding, m_decoder.get(), &AudioDecoder::startDecodingFilename);
        connect(m_decoder.get(), &AudioDecoder::progress, this, &TrackDeck::decodingProgress);
        connect(m_decoder.get(), &AudioDecoder::decodingFinished, this, &TrackDeck::decodingFinished);
        if (!m_filename.isEmpty())
            m_decoder->startDecodingFilename(m_filename);
    });
}

void TrackDeck::writeSamples(frame_t *begin, frame_t *end)
{
    m_player.writeSamples(begin, end);
}

void TrackDeck::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat("text/uri-list"))
        return;

    auto data = event->mimeData()->data("text/uri-list");

    QTextStream textStream(data, QIODevice::ReadOnly | QIODevice::Text);
    if (textStream.atEnd())
        return;

    const QUrl url(textStream.readLine());
    if (!url.isLocalFile())
        return;

    const QFileInfo fileInfo(url.toLocalFile());
    if (!fileInfo.exists())
        return;

    if (!fileInfo.isFile())
        return;

    event->acceptProposedAction();
}

void TrackDeck::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event)
}

void TrackDeck::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("text/uri-list"))
    {
        qWarning() << "wrong type";
        return;
    }

    auto data = event->mimeData()->data("text/uri-list");

    QTextStream textStream(data, QIODevice::ReadOnly | QIODevice::Text);
    if (textStream.atEnd())
    {
        qWarning() << "no lines";
        return;
    }

    const QUrl url(textStream.readLine());
    if (!url.isLocalFile())
    {
        qWarning() << "isnt local file";
        return;
    }

    const QFileInfo fileInfo(url.toLocalFile());
    if (!fileInfo.exists())
    {
        qWarning() << "doesnt exist";
        return;
    }

    if (!fileInfo.isFile())
    {
        qWarning() << "isnt file";
        return;
    }

    loadTrack(url.toLocalFile());
}

void TrackDeck::decodingProgress(int progress, int total)
{
    m_ui->progressBar->setMaximum(total);
    m_ui->progressBar->setValue(progress);
}

void TrackDeck::decodingFinished(const QAudioBuffer &buffer)
{
    m_player.setBuffer(buffer);

    m_ui->labelTitle->setText(QFileInfo{m_filename}.fileName());

    for (auto *btn : m_loopGroup.buttons())
        btn->setEnabled(true);

    m_ui->previewWidget->setBuffer(buffer);
    m_ui->scratchWidget->setBuffer(buffer);
    m_ui->progressBar->hide();
}

void TrackDeck::scratchBegin()
{
    disconnect(m_ui->sliderSpeed, &QAbstractSlider::valueChanged, this, &TrackDeck::speedChanged);
    m_ui->sliderSpeed->setEnabled(false);
    connect(m_ui->scratchWidget, &ScratchWidget::scratchSpeed, &m_player, &AudioPlayer::setSpeed);
    disconnect(&m_player, &AudioPlayer::playingChanged, this, &TrackDeck::updatePlayButtonText);

    m_playingBeforeScratch = m_player.playing();
    m_player.setPlaying(true);

    m_speedBeforeScratch = m_player.speed();

    m_stopOnEndBeforeScratch = m_player.stopOnEnd();
    m_player.setStopOnEnd(false);
}

void TrackDeck::scratchEnd()
{
    m_player.setPlaying(m_playingBeforeScratch);
    m_player.setSpeed(m_speedBeforeScratch);
    m_player.setStopOnEnd(m_stopOnEndBeforeScratch);

    disconnect(m_ui->scratchWidget, &ScratchWidget::scratchSpeed, &m_player, &AudioPlayer::setSpeed);
    m_ui->sliderSpeed->setEnabled(true);
    connect(m_ui->sliderSpeed, &QAbstractSlider::valueChanged, this, &TrackDeck::speedChanged);
    connect(&m_player, &AudioPlayer::playingChanged, this, &TrackDeck::updatePlayButtonText);
}

void TrackDeck::speedChanged(int value)
{
    m_player.setSpeed(float(value)/100.f);
}

void TrackDeck::updatePlayButtonText(bool playing)
{
    m_ui->pushButtonBpm->setEnabled(playing);
    m_ui->pushButtonPlay->setText(playing ? tr("▮▮") : tr("▶"));
}

void TrackDeck::bpmTap()
{
    const auto position = m_player.position();
    if (m_bpmTap)
    {
        std::get<1>(*m_bpmTap) = position;
        std::get<2>(*m_bpmTap)++;
        const auto framesPerBeat = (std::get<1>(*m_bpmTap)-std::get<0>(*m_bpmTap))/std::get<2>(*m_bpmTap);
        m_ui->scratchWidget->setFramesPerBeat(framesPerBeat);
        const auto beatsPerSecond = frameRate/framesPerBeat;
        const auto bpm = 60.*beatsPerSecond;
        m_ui->doubleSpinBoxBpm->setValue(bpm);
    }
    else
    {
        m_bpmTap = std::make_tuple(position, position, 0);
    }

    m_ui->pushButtonBpm->setText(QString::number(std::get<2>(*m_bpmTap)));
    m_timer.start();
}

void TrackDeck::timeout()
{
    const auto framesPerBeat = (std::get<1>(*m_bpmTap)-std::get<0>(*m_bpmTap))/std::get<2>(*m_bpmTap);
    m_ui->scratchWidget->setFramesPerBeat(framesPerBeat);
    const auto beatsPerSecond = frameRate/framesPerBeat;
    const auto bpm = 60.*beatsPerSecond;
    m_ui->pushButtonBpm->setText(tr("BPM tap"));
    m_ui->doubleSpinBoxBpm->setValue(bpm);
    m_bpmTap = {};
}

void TrackDeck::updatePlaybackBpm()
{
    m_ui->labelPlaybackBpm->setText(QString::number(m_ui->doubleSpinBoxBpm->value() * m_ui->sliderSpeed->value() / 100., 'f', 2));
}
