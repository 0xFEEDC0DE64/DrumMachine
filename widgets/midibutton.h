#pragma once

#include <QPushButton>

namespace midi { struct MidiMessage; }

class MidiButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(quint8 channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(quint8 note READ note WRITE setNote NOTIFY noteChanged)

public:
    explicit MidiButton(QWidget *parent = nullptr);
    explicit MidiButton(const QString &text, QWidget *parent = nullptr);
    MidiButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    quint8 channel() const { return m_channel; }
    void setChannel(quint8 channel) { if (channel == m_channel) return; emit channelChanged(m_channel = channel); }

    quint8 note() const { return m_note; }
    void setNote(quint8 note) { if (note == m_note) return; emit noteChanged(m_note = note); }

signals:
    void channelChanged(quint8 channel);
    void noteChanged(quint8 note);

public slots:
    void learn();
    void midiReceived(const midi::MidiMessage &message);

private:
    quint8 m_channel{99};
    quint8 m_note{99};

    bool m_learning{};
    QColor m_oldColor;
    QBrush m_oldBrush;
};
