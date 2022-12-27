#pragma once

#include <QWidget>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>

#include <memory>

#include "audioformat.h"
#include "midicontainers.h"
#include "treetotableproxymodel.h"

namespace Ui { class DjWidget; }
class DrumMachineSettings;

class DjWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DjWidget(QWidget *parent = nullptr);
    ~DjWidget() override;

    void injectDecodingThread(QThread &thread);
    void writeSamples(frame_t *begin, frame_t *end);
    void loadSettings(DrumMachineSettings &settings);
    void unsendColors();
    void sendColors();

signals:
    void sendMidi(const midi::MidiMessage &midiMsg);

public slots:
    void midiReceived(const midi::MidiMessage &message);

private slots:
    void directorySelected();

private:
    const std::unique_ptr<Ui::DjWidget> m_ui;

    QFileSystemModel m_directoryModel;
    QFileSystemModel m_filesModel;
    TreeToTableProxyModel m_filesTableModel;
    QSortFilterProxyModel m_sortFilterProxyModel;
};
