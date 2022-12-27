#pragma once

#include <QSplitter>
#include <QSortFilterProxyModel>

#include <memory>

#include "drumpadpresetsmodel.h"
#include "drumpadfilesmodel.h"

namespace Ui { class DrumPadWidget; }
class SequencerWidget;
class QModelIndex;
class QNetworkAccessManager;
class QThread;
class DrumMachineSettings;
class QNetworkReply;
namespace midi { struct MidiMessage; }
struct frame_t;

class DrumPadWidget : public QSplitter
{
    Q_OBJECT

public:
    explicit DrumPadWidget(QWidget *parent = nullptr);
    ~DrumPadWidget() override;

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

    const std::unique_ptr<Ui::DrumPadWidget> m_ui;

    DrumMachineSettings *m_settings{};

    DrumPadPresetsModel m_presetsModel;
    QSortFilterProxyModel m_presetsProxyModel;

    DrumPadFilesModel m_filesModel;

    QNetworkAccessManager *m_networkAccessManager{};

    std::unique_ptr<QNetworkReply> m_reply;
};
