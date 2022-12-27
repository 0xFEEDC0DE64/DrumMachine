#pragma once

#include <QSplitter>
#include <QSortFilterProxyModel>

#include <memory>

#include "loopstationpresetsmodel.h"

namespace Ui { class LoopStationWidget; }
namespace midi { struct MidiMessage; }
class QNetworkAccessManager;
class QNetworkReply;
class DrumMachineSettings;
struct frame_t;

class LoopStationWidget : public QSplitter
{
    Q_OBJECT

public:
    explicit LoopStationWidget(QWidget *parent = nullptr);
    ~LoopStationWidget() override;

    void writeSamples(frame_t *begin, frame_t *end);
    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);
    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void midiReceived(const midi::MidiMessage &message);

private slots:
    void currentRowChanged(const QModelIndex &current);
    void loadPresets();
    void requestFinished();
    void selectFirstPreset();
    void selectPrevPreset();
    void selectNextPreset();

private:
    void selectIndex(const QModelIndex &index);

    const std::unique_ptr<Ui::LoopStationWidget> m_ui;

    DrumMachineSettings *m_settings{};

    LoopStationPresetsModel m_presetsModel;
    QSortFilterProxyModel m_presetsProxyModel;

    QNetworkAccessManager *m_networkAccessManager{};

    std::unique_ptr<QNetworkReply> m_reply;
};
