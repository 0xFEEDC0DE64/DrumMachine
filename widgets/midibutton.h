#pragma once

#include <QPushButton>

#include "midilearnsetting.h"

namespace midi { struct MidiMessage; }
class QAction;

class MidiButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(MidiLearnSetting learnSetting READ learnSetting WRITE setLearnSetting NOTIFY learnSettingChanged)

public:
    explicit MidiButton(QWidget *parent = nullptr);
    explicit MidiButton(const QString &text, QWidget *parent = nullptr);
    MidiButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    MidiLearnSetting learnSetting() const { return m_learnSetting; }
    void setLearnSetting(const MidiLearnSetting &learnSetting);

signals:
    void learnSettingChanged(const MidiLearnSetting &learnSetting);

public slots:
    void learn();
    void midiReceived(const midi::MidiMessage &message);

private:
    MidiLearnSetting m_learnSetting;

    bool m_learning{};
    QColor m_oldColor;
    QBrush m_oldBrush;

    QAction *m_actionCmd{};
    QAction *m_actionChannel{};
    QAction *m_actionNote{};
};
