#pragma once

#include <memory>

#include <QFrame>

#include "presets.h"

namespace Ui { class SampleWidget; }
class QThread;
class QAudioDeviceInfo;
class QSoundEffect;

class SampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SampleWidget(QWidget *parent = nullptr);
    ~SampleWidget() override;

    void setFile(const QString &presetId, const presets::File &file);

    quint8 channel() const;
    void setChannel(quint8 channel);

    quint8 note() const;
    void setNote(quint8 note);

    std::optional<int> choke() const;

    void pressed(quint8 velocity);
    void released();

    void forceStop();

    void setupAudioThread(const QAudioDeviceInfo &device, QThread &thread);

signals:
    void chokeTriggered(int choke);

private slots:
    void updateStatus();

private:
    QUrl sampleUrl() const;

    const std::unique_ptr<Ui::SampleWidget> m_ui;

    QString m_presetId;
    std::optional<presets::File> m_file;

    std::unique_ptr<QSoundEffect> m_effect;
};
