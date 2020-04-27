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
#include <QDebug>

#include "audiodecoder.h"

TrackDeck::TrackDeck(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::TrackDeck>()}
{
    m_ui->setupUi(this);
    m_ui->progressBar->hide();

    connect(m_ui->pushButtonPlay, &QAbstractButton::pressed, &m_player, &AudioPlayer::togglePlaying);
    connect(m_ui->pushButtonStop, &QAbstractButton::pressed, &m_player, &AudioPlayer::stop);

    connect(m_ui->verticalSliderVolume, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setVolume(float(value)/100.f); });

    connect(m_ui->horizontalSliderSpeed, &QAbstractSlider::valueChanged, &m_player, [&player=m_player](int value){ player.setSpeed(float(value)/100.f); });

    connect(m_ui->previewWidget, &PreviewWidget::positionSelected, &m_player, &AudioPlayer::setPosition);
    connect(&m_player, &AudioPlayer::positionChanged, m_ui->previewWidget, &PreviewWidget::setPosition);
    connect(&m_player, &AudioPlayer::positionChanged, m_ui->scratchWidget, &ScratchWidget::setPosition);
    connect(&m_player, &AudioPlayer::playingChanged, m_ui->pushButtonPlay, [&button=*m_ui->pushButtonPlay](bool playing){ button.setText(playing ? tr("▮▮") : tr("▶")); });
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
    m_ui->previewWidget->setBuffer(buffer);
    m_ui->scratchWidget->setBuffer(buffer);
    m_ui->progressBar->hide();
}
