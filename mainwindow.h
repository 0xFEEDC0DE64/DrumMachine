#pragma once

#include <memory>

#include <QMainWindow>
#include <QSortFilterProxyModel>

#include "presetsmodel.h"
#include "filesmodel.h"
#include "midiinwrapper.h"

namespace Ui { class MainWindow; }
namespace presets { struct PresetsConfig; }
namespace midi { struct MidiMessage; }
class QAudioDeviceInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const presets::PresetsConfig &presetsConfig, QWidget *parent = nullptr);
    ~MainWindow() override;

    void selectFirstPreset();

private slots:
    void messageReceived(const midi::MidiMessage &message);
    void currentRowChanged(const QModelIndex &current);

private:
    void updateMidiDevices();
    void updateAudioDevices();

    const std::unique_ptr<Ui::MainWindow> m_ui;

    MidiInWrapper m_midiIn;

    QList<QAudioDeviceInfo> m_devices;

    PresetsModel m_presetsModel;
    QSortFilterProxyModel m_presetsProxyModel;

    FilesModel m_filesModel;
};
