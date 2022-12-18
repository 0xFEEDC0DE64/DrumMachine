#pragma once

#include <QSplitter>
#include <QSortFilterProxyModel>

#include <memory>

#include "audioformat.h"
#include "midicontainers.h"
#include "presetsmodel.h"
#include "filesmodel.h"

namespace Ui { class DrumPadWidget; }
class SamplesWidget;
class SequencerWidget;
class QModelIndex;
class QNetworkAccessManager;
class QThread;
class DrumMachineSettings;
class QNetworkReply;

class DrumPadWidget : public QSplitter
{
    Q_OBJECT

public:
    explicit DrumPadWidget(QWidget *parent = nullptr);
    ~DrumPadWidget() override;

    void selectFirstPreset();
    void writeSamples(frame_t *begin, frame_t *end);
    void injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager);
    void injectDecodingThread(QThread &thread);
    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void messageReceived(const midi::MidiMessage &message);

private slots:
    void currentRowChanged(const QModelIndex &current);
    void loadPresets();
    void requestFinished();

private:
    const std::unique_ptr<Ui::DrumPadWidget> m_ui;

    PresetsModel m_presetsModel;
    QSortFilterProxyModel m_presetsProxyModel;

    FilesModel m_filesModel;

    QNetworkAccessManager *m_networkAccessManager{};

    std::unique_ptr<QNetworkReply> m_reply;
};
