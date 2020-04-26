#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaEnum>
#include <QMessageBox>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QAudioDeviceInfo>
#include <QDebug>

#include "presets.h"
#include "midiinwrapper.h"
#include "midicontainers.h"

namespace {
void DummyDeleter(PaStream *stream) {}
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

    static_cast<MainWindow*>(userData)->writeSamples(begin, begin+framesPerBuffer);
}
}

MainWindow::MainWindow(const presets::PresetsConfig &presetsConfig, QWidget *parent) :
    QMainWindow{parent},
    m_ui{std::make_unique<Ui::MainWindow>()},
    m_paStream(nullptr, DummyDeleter),
    m_presetsModel{*presetsConfig.presets}
{
    m_ui->setupUi(this);
    m_ui->splitter->setSizes({99999, 1});

    connect(&m_midiIn, &MidiInWrapper::messageReceived, this, &MainWindow::messageReceived);

    connect(m_ui->sequencerWidget, &SequencerWidget::triggerSample, m_ui->samplesWidget, &SamplesWidget::sequencerTriggerSample);

    updateMidiDevices();

    connect(m_ui->pushButtonMidiController, &QAbstractButton::pressed, this, [this](){
        if (m_midiIn.isPortOpen())
            m_midiIn.closePort();
        else
        {
            const auto index = m_ui->comboBoxMidiController->currentIndex();
            if (index != -1)
                m_midiIn.openPort(index);
        }

        m_ui->pushButtonMidiController->setText(m_midiIn.isPortOpen() ? tr("Close") : tr("Open"));
    });

    updateAudioDevices();

    m_ui->comboBoxAudioDevice->setCurrentIndex(Pa_GetDefaultOutputDevice());

    connect(m_ui->pushButtonAudioDevice, &QAbstractButton::pressed, this, &MainWindow::openAudioDevice);

    m_presetsProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_presetsProxyModel.setSourceModel(&m_presetsModel);
    m_ui->presetsView->setModel(&m_presetsProxyModel);

    m_presetsProxyModel.setFilterKeyColumn(1);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, this, [=](){
        m_presetsProxyModel.setFilterFixedString(m_ui->lineEdit->text());
    });

    m_ui->filesView->setModel(&m_filesModel);

    connect(m_ui->presetsView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
}

MainWindow::~MainWindow() = default;

void MainWindow::selectFirstPreset()
{
    if (m_presetsProxyModel.rowCount())
    {
        const auto index = m_presetsProxyModel.index(0, 0);
        if (index.isValid())
        {
            m_ui->presetsView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            currentRowChanged(index);
        }
    }
}

void MainWindow::writeSamples(frame_t *begin, frame_t *end)
{
    std::fill(begin, end, frame_t{0.,0.});

    m_ui->samplesWidget->writeSamples(begin, end);
}

void MainWindow::openAudioDevice()
{
    if (m_paStream)
    {
        m_paStream = nullptr;
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

        if (PaError err = Pa_OpenStream(&stream, NULL, &outputParameters, sampleRate, m_ui->spinBoxBufferSize->value(), paNoFlag, &paCallback, this); err != paNoError)
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
        m_ui->pushButtonAudioDevice->setText(tr("Close"));
    }
}

void MainWindow::messageReceived(const midi::MidiMessage &message)
{
    m_ui->statusbar->showMessage(tr("Received midi message: flag: %0 cmd: %1 channel: %2 note: %3 velocity: %4")
                                 .arg(message.flag?"true":"false", QMetaEnum::fromType<midi::Command>().valueToKey(int(message.cmd)))
                                 .arg(message.channel).arg(message.note).arg(message.velocity), 1000);

    m_ui->samplesWidget->messageReceived(message);
}

void MainWindow::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
        return;

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    m_ui->presetDetailWidget->setPreset(preset);
    m_filesModel.setPreset(preset);
    m_ui->sequencerWidget->setPreset(preset);
    m_ui->samplesWidget->setPreset(preset);
}

void MainWindow::updateMidiDevices()
{
    m_ui->comboBoxMidiController->clear();

    for (const auto &name : m_midiIn.portNames())
        m_ui->comboBoxMidiController->addItem(name);

    m_ui->pushButtonMidiController->setEnabled(m_ui->comboBoxMidiController->count() > 0);
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
