#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEventLoop>
#include <QMetaEnum>
#include <QMessageBox>
#include <QAbstractEventDispatcher>
#include <QAudioDeviceInfo>
#include <QDebug>

#include "audioformat.h"
#include "midicontainers.h"
#include "settingsdialog.h"

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
    m_settings{this},
    m_paStream{nullptr, DummyDeleter},
    m_midiIn{RtMidi::UNSPECIFIED, "DrumMachine", 100, this},
    m_midiOut{RtMidi::UNSPECIFIED, "DrumMachine", this},
    m_networkAccessManager{this},
    m_cache{this},
    m_decoderThread{this}
{
    m_ui->setupUi(this);

    m_cache.setCacheDirectory(m_settings.cacheDir());
    m_cache.setMaximumCacheSize(m_settings.maximumCacheSize());

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

    connect(m_ui->pushButtonSettings, &QAbstractButton::pressed, this, &MainWindow::showSettings);

    updateAudioDevices();
    connect(m_ui->pushButtonRefreshAudioDevices, &QAbstractButton::pressed, this, &MainWindow::updateAudioDevices);
    m_ui->spinBoxBufferSize->setValue(m_settings.framesPerBuffer());
    if (const auto &lastAudioDevice = m_settings.lastAudioDevice(); !lastAudioDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxAudioDevice->findText(lastAudioDevice); index >= 0)
        {
            m_ui->comboBoxAudioDevice->setCurrentIndex(index);
            if (m_settings.autoOpenAudioDevice())
                openAudioDevice();
        }
        else
            goto paDefault;
    }
    else
    {
paDefault:
        m_ui->comboBoxAudioDevice->setCurrentIndex(Pa_GetDefaultOutputDevice());
    }
    connect(m_ui->pushButtonAudioDevice, &QAbstractButton::pressed, this, &MainWindow::openAudioDevice);

    updateMidiInDevices();    
    connect(m_ui->pushButtonRefreshMidiIn, &QAbstractButton::pressed, this, &MainWindow::updateMidiInDevices);
    if (const auto &lastMidiInDevice = m_settings.lastMidiInDevice(); !lastMidiInDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxMidiIn->findText(lastMidiInDevice); index >= 0)
        {
            m_ui->comboBoxMidiIn->setCurrentIndex(index);
            if (m_settings.autoOpenMidiIn())
                openMidiInDevice();
        }
    }
    connect(m_ui->pushButtonMidiIn, &QAbstractButton::pressed, this, &MainWindow::openMidiInDevice);

    updateMidiOutDevices();
    connect(m_ui->pushButtonRefreshMidiOut, &QAbstractButton::pressed, this, &MainWindow::updateMidiOutDevices);
    if (const auto &lastMidiOutDevice = m_settings.lastMidiOutDevice(); !lastMidiOutDevice.isEmpty())
    {
        if (const auto index = m_ui->comboBoxMidiOut->findText(lastMidiOutDevice); index >= 0)
        {
            m_ui->comboBoxMidiOut->setCurrentIndex(index);
            if (m_settings.autoOpenMidiOut())
                openMidiOutDevice();
        }
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

void MainWindow::showSettings()
{
    SettingsDialog dialog{m_settings, m_cache.cacheSize(), this};
    if (dialog.exec() == QDialog::Accepted)
    {
        m_cache.setCacheDirectory(m_settings.cacheDir());
        m_cache.setMaximumCacheSize(m_settings.maximumCacheSize());
    }
}

void MainWindow::openAudioDevice()
{
    if (m_paStream)
    {
        m_paStream = nullptr;
        m_ui->comboBoxAudioDevice->setEnabled(true);
        m_ui->pushButtonRefreshAudioDevices->setEnabled(true);
        m_ui->spinBoxBufferSize->setEnabled(true);
        m_ui->pushButtonAudioDevice->setText(tr("Open"));
        m_settings.setAutoOpenAudioDevice(false);
    }
    else
    {
        PaDeviceIndex index = m_ui->comboBoxAudioDevice->currentIndex();
        if (index < 0)
        {
            QMessageBox::warning(this, tr("No audio device selected!"), tr("No audio device selected!"));
            return;
        }

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
            QMessageBox::warning(this, tr("Error opening audio device!"), tr("Error opening audio device!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        auto smartPtr = std::unique_ptr<PaStream, void(*)(PaStream*)>(stream, PaStreamCloser);
        stream = nullptr;

        if (PaError err = Pa_SetStreamFinishedCallback(smartPtr.get(), &paStreamFinished); err != paNoError)
        {
            QMessageBox::warning(this, tr("Error opening audio device!"), tr("Error setting finished callback!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        if (PaError err = Pa_StartStream(smartPtr.get()); err != paNoError)
        {
            QMessageBox::warning(this, tr("Error opening audio device!"), tr("Error starting stream!") + "\n\n" + Pa_GetErrorText(err));
            return;
        }

        // stream has been started and from now on we not only need to delete it, but also stop it first
        smartPtr.get_deleter() = PaStreamStopperAndCloser;

        m_paStream = std::move(smartPtr);
        m_ui->comboBoxAudioDevice->setEnabled(false);
        m_ui->pushButtonRefreshAudioDevices->setEnabled(false);
        m_ui->spinBoxBufferSize->setEnabled(false);
        m_ui->pushButtonAudioDevice->setText(tr("Close"));
        m_settings.setLastAudioDevice(m_ui->comboBoxAudioDevice->currentText());
        m_settings.setFramesPerBuffer(m_ui->spinBoxBufferSize->value());
        m_settings.setAutoOpenAudioDevice(true);
    }
}

void MainWindow::openMidiInDevice()
{
    if (m_midiIn.isPortOpen())
    {
        m_midiIn.closePort();

        if (m_midiIn.isPortOpen())
        {
            QMessageBox::warning(this, tr("Could not close midi in device!"), tr("Could not close midi in device!"));
            return;
        }

        m_ui->comboBoxMidiIn->setEnabled(true);
        m_ui->pushButtonRefreshMidiIn->setEnabled(true);
        m_ui->pushButtonMidiIn->setText(tr("Open"));
        m_settings.setAutoOpenMidiIn(false);
    }
    else
    {
        const auto index = m_ui->comboBoxMidiIn->currentIndex();
        if (index < 0)
        {
            QMessageBox::warning(this, tr("No midi in device selected!"), tr("No midi in device selected!"));
            return;
        }

        m_midiIn.openPort(index, "Input");

        if (!m_midiIn.isPortOpen())
        {
            QMessageBox::warning(this, tr("Could not open midi in device!"), tr("Could not open midi in device!"));
            return;
        }

        m_ui->comboBoxMidiIn->setEnabled(false);
        m_ui->pushButtonRefreshMidiIn->setEnabled(false);
        m_ui->pushButtonMidiIn->setText(tr("Close"));
        m_settings.setLastMidiInDevice(m_ui->comboBoxMidiIn->currentText());
        m_settings.setAutoOpenMidiIn(true);
    }
}

void MainWindow::openMidiOutDevice()
{
    if (m_midiOut.isPortOpen())
    {
        unsendColors(m_ui->tabWidget->currentIndex());

        m_midiOut.closePort();

        if (m_midiOut.isPortOpen())
        {
            QMessageBox::warning(this, tr("Could not close midi out device!"), tr("Could not close midi out device!"));
            return;
        }

        m_ui->comboBoxMidiOut->setEnabled(true);
        m_ui->pushButtonRefreshMidiOut->setEnabled(true);
        m_ui->pushButtonMidiOut->setText(tr("Open"));
        m_settings.setAutoOpenMidiOut(false);
    }
    else
    {
        const auto index = m_ui->comboBoxMidiOut->currentIndex();
        if (index < 0)
        {
            QMessageBox::warning(this, tr("No midi out device selected!"), tr("No midi out device selected!"));
            return;
        }

        m_midiOut.openPort(index, "Output");

        if (!m_midiOut.isPortOpen())
        {
            QMessageBox::warning(this, tr("Could not open midi out device!"), tr("Could not open midi out device!"));
            return;
        }

        m_ui->comboBoxMidiOut->setEnabled(false);
        m_ui->pushButtonRefreshMidiOut->setEnabled(false);
        m_ui->pushButtonMidiOut->setText(tr("Close"));
        m_settings.setLastMidiOutDevice(m_ui->comboBoxMidiOut->currentText());
        m_settings.setAutoOpenMidiOut(true);
        sendColors(m_ui->tabWidget->currentIndex());
    }
}

void MainWindow::midiReceived(const midi::MidiMessage &message)
{
    m_ui->statusbar->showMessage(tr("Received midi message: flag: %0 cmd: %1 channel: %2 note: %3 velocity: %4")
                                 .arg(message.flag?"true":"false", QMetaEnum::fromType<midi::Command>().valueToKey(int(message.cmd)))
                                 .arg(message.channel).arg(message.note).arg(message.velocity), 1000);

    m_ui->tabWidget->midiReceived(message);

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
    for (int i = 0; i < m_ui->tabWidget->count(); i++)
        m_ui->tabWidget->setLearnSetting(i, m_settings.tabWidget(i));
    connect(m_ui->tabWidget, &MidiTabWidget::learnSettingChanged, &m_settings, &DrumMachineSettings::setTabWidget);

    m_ui->drumPadWidget->loadSettings(m_settings);
    m_ui->loopStationWidget->loadSettings(m_settings);
    m_ui->djWidget->loadSettings(m_settings);
    m_ui->synthisizerWidget->loadSettings(m_settings);
}

void MainWindow::unsendColors(int index)
{
    {
        const auto &learnSetting = m_ui->tabWidget->learnSetting(index);
        emit sendMidi(midi::MidiMessage {
            .channel = learnSetting.channel,
            .cmd = learnSetting.cmd,
            .flag = true,
            .note = learnSetting.note,
            .velocity = m_settings.colorOff()
        });
    }

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
    if (true)
    {
        {
            const auto &learnSetting = m_ui->tabWidget->learnSetting(index);
            emit sendMidi(midi::MidiMessage {
                .channel = learnSetting.channel,
                .cmd = learnSetting.cmd,
                .flag = true,
                .note = learnSetting.note,
                .velocity = m_settings.colorTabWidget()
            });
        }

        if (index == 0)
            m_ui->drumPadWidget->sendColors();
        else if (index == 1)
            m_ui->loopStationWidget->sendColors();
        else if (index == 2)
            m_ui->djWidget->sendColors();
        else if (index == 3)
            m_ui->synthisizerWidget->sendColors();

        return;
    }

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
