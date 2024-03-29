#pragma once

#include <memory>

#include <QMainWindow>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

#include "portaudio.h"

#include "midiinwrapper.h"
#include "midioutwrapper.h"
#include "drummachinesettings.h"

namespace Ui { class MainWindow; }
namespace midi { struct MidiMessage; }
struct frame_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    int writeSamples(frame_t *begin, frame_t *end);

private slots:
    void showSettings();
    void openAudioDevice();
    void openMidiInDevice();
    void openMidiOutDevice();
    void midiReceived(const midi::MidiMessage &message);
    void onQuarterNote();
    void sendMidi(const midi::MidiMessage &midiMsg);
    void currentChanged(int index);

private:
    void updateAudioDevices();
    void updateMidiInDevices();
    void updateMidiOutDevices();
    void loadSettings();
    void unsendColors(int index);
    void sendColors(int index);

    const std::unique_ptr<Ui::MainWindow> m_ui;

    DrumMachineSettings m_settings;

    std::unique_ptr<PaStream, void(*)(PaStream*)> m_paStream;

    MidiInWrapper m_midiIn;
    MidiOutWrapper m_midiOut;

    QNetworkAccessManager m_networkAccessManager;
    QNetworkDiskCache m_cache;

    QThread m_decoderThread;

    int m_lastIndex;
};
