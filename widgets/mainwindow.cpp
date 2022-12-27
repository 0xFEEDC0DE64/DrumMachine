#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEventLoop>
#include <QMetaEnum>
#include <QMessageBox>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QAudioDeviceInfo>
#include <QDebug>

#include "audioformat.h"
#include "midicontainers.h"

namespace {
void DummyDeleter(PaStream *stream);
void PaStreamCloser(PaStream *stream);
void PaStreamStopperAndCloser(PaStream *stream);
void paStreamFinished(void* userData);
int paCallback(const void *inputBuffer, void *outputBuffer,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags,
               void *userData);
} // namespace

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    m_ui{std::make_unique<Ui::MainWindow>()},
    m_paStream{nullptr, DummyDeleter},
    m_midiIn{RtMidi::UNSPECIFIED, "DrumMachine"},
    m_midiOut{RtMidi::UNSPECIFIED, "DrumMachine"}
{
    m_ui->setupUi(this);

    m_cache.setCacheDirectory("cache");
    m_cache.setMaximumCacheSize(2ull * 1024 * 1024 * 1024);
    m_networkAccessManager.setCache(&m_cache);

    m_ui->drumPadWidget->injectNetworkAccessManager(m_networkAccessManager);
    m_ui->loopStationWidget->injectNetworkAccessManager(m_networkAccessManager);

    connect(&m_midiIn, &MidiInWrapper::midiReceived, this, &MainWindow::midiReceived);

    {
        QEventLoop eventLoop;
        connect(&m_decoderThread, &QThread::started, &eventLoop, &QEventLoop::quit);
        m_decoderThread.start(QThread::HighestPriority);
        eventLoop.exec();
    }

    m_ui->drumPadWidget->injectDecodingThread(m_decoderThread);
    m_ui->loopStationWidget->injectDecodingThread(m_decoderThread);
    m_ui->djWidget->injectDecodingThread(m_decoderThread);

    updateAudioDevices();
    connect(m_ui->pushButtonRefreshAudioDevices, &QAbstractButton::pressed, this, &MainWindow::updateAudioDevices);
    if (const auto &lastAudioDevice = m_settings.lastAudioDevice(); !lastAudioDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxAudioDevice->findText(lastAudioDevice); index >= 0)
            m_ui->comboBoxAudioDevice->setCurrentIndex(index);
        else
            goto paDefault;
    }
    else
    {
paDefault:
        m_ui->comboBoxAudioDevice->setCurrentIndex(Pa_GetDefaultOutputDevice());
    }
    m_ui->spinBoxBufferSize->setValue(m_settings.framesPerBuffer());
    connect(m_ui->pushButtonAudioDevice, &QAbstractButton::pressed, this, &MainWindow::openAudioDevice);

    updateMidiInDevices();    
    connect(m_ui->pushButtonRefreshMidiIn, &QAbstractButton::pressed, this, &MainWindow::updateMidiInDevices);
    if (const auto &lastMidiInDevice = m_settings.lastMidiInDevice(); !lastMidiInDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxMidiIn->findText(lastMidiInDevice); index >= 0)
            m_ui->comboBoxMidiIn->setCurrentIndex(index);
    }
    connect(m_ui->pushButtonMidiIn, &QAbstractButton::pressed, this, &MainWindow::openMidiInDevice);

    updateMidiOutDevices();
    connect(m_ui->pushButtonRefreshMidiOut, &QAbstractButton::pressed, this, &MainWindow::updateMidiOutDevices);
    if (const auto &lastMidiOutDevice = m_settings.lastMidiOutDevice(); !lastMidiOutDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxMidiOut->findText(lastMidiOutDevice); index >= 0)
            m_ui->comboBoxMidiOut->setCurrentIndex(index);
    }
    connect(m_ui->pushButtonMidiOut, &QAbstractButton::pressed, this, &MainWindow::openMidiOutDevice);

    loadSettings();

    connect(m_ui->drumPadWidget, &DrumPadWidget::sendMidi, this, &MainWindow::sendMidi);
    connect(m_ui->loopStationWidget, &LoopStationWidget::sendMidi, this, &MainWindow::sendMidi);
    connect(m_ui->djWidget, &DjWidget::sendMidi, this, &MainWindow::sendMidi);
    connect(m_ui->synthisizerWidget, &SynthisizerWidget::sendMidi, this, &MainWindow::sendMidi);

    m_lastIndex = m_ui->tabWidget->currentIndex();
    connect(m_ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::currentChanged);
}

MainWindow::~MainWindow()
{
    m_paStream = nullptr;

    m_decoderThread.exit();
    m_decoderThread.wait();
}

int MainWindow::writeSamples(frame_t *begin, frame_t *end)
{
    std::fill(begin, end, frame_t{0.,0.});

    m_ui->drumPadWidget->writeSamples(begin, end);
    m_ui->loopStationWidget->writeSamples(begin, end);
    m_ui->djWidget->writeSamples(begin, end);
    m_ui->synthisizerWidget->writeSamples(begin, end);

    std::transform(begin, end, begin, [factor=float(m_ui->horizontalSliderMaster->value())/100.f](frame_t frame){
        std::transform(std::cbegin(frame), std::cend(frame), std::begin(frame), [&factor](const sample_t &sample){
            return sample*factor;
        });
        return frame;
    });

    return paContinue;
}

void MainWindow::openAudioDevice()
{
    if (m_paStream)
    {
        m_paStream = nullptr;
        m_ui->comboBoxAudioDevice->setEnabled(true);
        m_ui->spinBoxBufferSize->setEnabled(true);
        m_ui->pushButtonAudioDevice->setText(tr("Open"));
    }
    else
    {
        PaDeviceIndex index = m_ui->comboBoxAudioDevice->currentIndex();

        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);

        const PaStreamParameters outputParameters {
            .device = index,
            .channelCount = channelCount,
            .sampleFormat = paFloat32, /* 32 bit floating point output */
            .suggestedLatency = pInfo->defaultLowOutputLatency,
            .hostApiSpecificStreamInfo = NULL
        };

        PaStream *stream{};

        if (PaError err = Pa_OpenStream(&stream, NULL, &outputParameters, frameRate, m_ui->spinBoxBufferSize->value(), paNoFlag, &paCallback, this); err != paNoError)
        {
            QMessageBox::warning(this, tr("Error opening stream!"), tr("Error opening stream!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        auto smartPtr = std::unique_ptr<PaStream, void(*)(PaStream*)>(stream, PaStreamCloser);
        stream = nullptr;

        if (PaError err = Pa_SetStreamFinishedCallback(smartPtr.get(), &paStreamFinished); err != paNoError)
        {
            QMessageBox::warning(this, tr("Error setting finished callback!"), tr("Error setting finished callback!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        if (PaError err = Pa_StartStream(smartPtr.get()); err != paNoError)
        {
            QMessageBox::warning(this, tr("Error starting stream!"), tr("Error starting stream!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        // stream has been started and from now on we not only need to delete it, but also stop it first
        smartPtr.get_deleter() = PaStreamStopperAndCloser;

        m_paStream = std::move(smartPtr);
        m_ui->comboBoxAudioDevice->setEnabled(false);
        m_ui->spinBoxBufferSize->setEnabled(false);
        m_ui->pushButtonAudioDevice->setText(tr("Close"));

        m_settings.setLastAudioDevice(m_ui->comboBoxAudioDevice->currentText());
        m_settings.setFramesPerBuffer(m_ui->spinBoxBufferSize->value());
    }
}

void MainWindow::openMidiInDevice()
{
    if (m_midiIn.isPortOpen())
        m_midiIn.closePort();
    else
    {
        const auto index = m_ui->comboBoxMidiIn->currentIndex();
        if (index != -1)
            m_midiIn.openPort(index, "Input");
        m_settings.setLastMidiInDevice(m_ui->comboBoxMidiIn->currentText());
    }

    m_ui->comboBoxMidiIn->setDisabled(m_midiIn.isPortOpen());
    m_ui->pushButtonMidiIn->setText(m_midiIn.isPortOpen() ? tr("Close") : tr("Open"));
}

void MainWindow::openMidiOutDevice()
{
    if (m_midiOut.isPortOpen())
    {
        qDebug() << "closing port";
        unsendColors(m_ui->tabWidget->currentIndex());
        m_midiOut.closePort();
    }
    else
    {
        const auto index = m_ui->comboBoxMidiOut->currentIndex();
        if (index != -1)
        {
            m_midiOut.openPort(index, "Output");
            m_settings.setLastMidiOutDevice(m_ui->comboBoxMidiOut->currentText());
            sendColors(m_ui->tabWidget->currentIndex());
        }
    }

    m_ui->comboBoxMidiOut->setDisabled(m_midiOut.isPortOpen());
    m_ui->pushButtonMidiOut->setText(m_midiOut.isPortOpen() ? tr("Close") : tr("Open"));
}

void MainWindow::midiReceived(const midi::MidiMessage &message)
{
    m_ui->statusbar->showMessage(tr("Received midi message: flag: %0 cmd: %1 channel: %2 note: %3 velocity: %4")
                                 .arg(message.flag?"true":"false", QMetaEnum::fromType<midi::Command>().valueToKey(int(message.cmd)))
                                 .arg(message.channel).arg(message.note).arg(message.velocity), 1000);

    if (m_ui->tabWidget->currentIndex() == 0)
        m_ui->drumPadWidget->midiReceived(message);
    else if (m_ui->tabWidget->currentIndex() == 1)
        m_ui->loopStationWidget->midiReceived(message);
    else if (m_ui->tabWidget->currentIndex() == 2)
        m_ui->djWidget->midiReceived(message);
    else if (m_ui->tabWidget->currentIndex() == 3)
        m_ui->synthisizerWidget->midiReceived(message);
}

void MainWindow::sendMidi(const midi::MidiMessage &midiMsg)
{
    if (m_midiOut.isPortOpen())
        m_midiOut.sendMessage(midiMsg);
}

void MainWindow::currentChanged(int index)
{
    unsendColors(m_lastIndex);
    m_lastIndex = index;
    sendColors(index);
}

void MainWindow::updateAudioDevices()
{
    m_ui->comboBoxAudioDevice->clear();

    const auto count = Pa_GetDeviceCount();

    for (PaDeviceIndex i = 0; i < count; i++)
    {
        const auto info = Pa_GetDeviceInfo(i);
        m_ui->comboBoxAudioDevice->addItem(info->name);
    }
}

void MainWindow::updateMidiInDevices()
{
    m_ui->comboBoxMidiIn->clear();

    for (const auto &name : m_midiIn.portNames())
        m_ui->comboBoxMidiIn->addItem(name);

    m_ui->pushButtonMidiIn->setEnabled(m_ui->comboBoxMidiIn->count() > 0);
}

void MainWindow::updateMidiOutDevices()
{
    m_ui->comboBoxMidiOut->clear();

    for (const auto &name : m_midiOut.portNames())
        m_ui->comboBoxMidiOut->addItem(name);

    m_ui->pushButtonMidiOut->setEnabled(m_ui->comboBoxMidiOut->count() > 0);
}

void MainWindow::loadSettings()
{
    m_ui->drumPadWidget->loadSettings(m_settings);
    m_ui->loopStationWidget->loadSettings(m_settings);
    m_ui->djWidget->loadSettings(m_settings);
    m_ui->synthisizerWidget->loadSettings(m_settings);
}

void MainWindow::unsendColors(int index)
{
    if (index == 0)
        m_ui->drumPadWidget->unsendColors();
    else if (index == 1)
        m_ui->loopStationWidget->unsendColors();
    else if (index == 2)
        m_ui->djWidget->unsendColors();
    else if (index == 3)
        m_ui->synthisizerWidget->unsendColors();
}

void MainWindow::sendColors(int index)
{
    if (index == 0)
        m_ui->drumPadWidget->sendColors();
    else if (index == 1)
        m_ui->loopStationWidget->sendColors();
    else if (index == 2)
        m_ui->djWidget->sendColors();
    else if (index == 3)
        m_ui->synthisizerWidget->sendColors();

    return;

    // this was just for debugging all the available colors on novation launchpad mk1

    int k{0};
    for (int j = 0; j < 128; j+= 16)
    {
        for (auto i = 0; i < 8; i++)
        {
            midi::MidiMessage midiMsg;
            midiMsg.channel = 0;
            midiMsg.cmd = midi::Command::NoteOn;
            midiMsg.flag = true;
            midiMsg.note = i + j;
            midiMsg.velocity = k++;
            sendMidi(midiMsg);
        }
    }
}

namespace {
void DummyDeleter(PaStream *stream)
{
    Q_UNUSED(stream);
}
void PaStreamCloser(PaStream *stream)
{
    if (PaError err = Pa_CloseStream(stream); err != paNoError)
        fprintf(stderr, "Could not close stream!\n");
}
void PaStreamStopperAndCloser(PaStream *stream)
{
    if (PaError err = Pa_StopStream(stream); err != paNoError)
        fprintf(stderr, "Could not stop stream!\n");
    PaStreamCloser(stream);
}

void paStreamFinished(void* userData)
{
    Q_UNUSED(userData)
    printf("Stream Completed\n");
}

int paCallback(const void *inputBuffer, void *outputBuffer,
               unsigned long framesPerBuffer,
               const PaStreamCallbackTimeInfo* timeInfo,
               PaStreamCallbackFlags statusFlags,
               void *userData)
{
    Q_UNUSED(inputBuffer)
    Q_ASSERT(outputBuffer);
    Q_ASSERT(framesPerBuffer);
    Q_UNUSED(timeInfo)
    Q_UNUSED(statusFlags)
    Q_ASSERT(userData);

    auto begin = static_cast<frame_t*>(outputBuffer);

    return static_cast<MainWindow*>(userData)->writeSamples(begin, begin+framesPerBuffer);
}
} // namespace
