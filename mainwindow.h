#pragma once

#include <memory>

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QThread>

#include "portaudio.h"

#include "audioformat.h"
#include "presetsmodel.h"
#include "filesmodel.h"
#include "midiinwrapper.h"
#include "midioutwrapper.h"
#include "synthisizer.h"
#include "drummachinesettings.h"

namespace Ui { class MainWindow; }
namespace presets { struct PresetsConfig; }
namespace midi { struct MidiMessage; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const presets::PresetsConfig &presetsConfig, QWidget *parent = nullptr);
    ~MainWindow() override;

    void selectFirstPreset();

    void writeSamples(frame_t *begin, frame_t *end);

private slots:
    void openAudioDevice();
    void messageReceived(const midi::MidiMessage &message);
    void currentRowChanged(const QModelIndex &current);
    void sendMidi(const midi::MidiMessage &midiMsg);

private:
    void updateMidiInDevices();
    void updateMidiOutDevices();
    void updateAudioDevices();
    void loadSettings();
    void sendColors();

    const std::unique_ptr<Ui::MainWindow> m_ui;

    DrumMachineSettings m_settings;

    std::unique_ptr<PaStream, void(*)(PaStream*)> m_paStream;

    MidiInWrapper m_midiIn;
    MidiOutWrapper m_midiOut;

    QThread m_decoderThread;

    Synthisizer m_synthisizer;

    PresetsModel m_presetsModel;
    QSortFilterProxyModel m_presetsProxyModel;

    FilesModel m_filesModel;
};
