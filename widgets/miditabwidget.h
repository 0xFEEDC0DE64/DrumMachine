#pragma once

#include <QTabWidget>

#include <optional>
#include <vector>

namespace midi { struct MidiMessage; }

class MidiTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MidiTabWidget(QWidget *parent = nullptr);

    quint8 channel(int index) const;
    void setChannel(int index, quint8 channel);

    quint8 note(int index) const;
    void setNote(int index, quint8 note);

signals:
    void channelChanged(int index, quint8 channel);
    void noteChanged(int index, quint8 note);

public slots:
    void learn(int index);
    void midiReceived(const midi::MidiMessage &message);

protected:
    void tabInserted(int index) override;
    void tabRemoved(int index) override;

private slots:
    void showContextMenu(const QPoint &pos);

private:
    std::optional<int> m_learning;
    QColor m_oldColor;

    struct ChannelNote
    {
        quint8 channel{99};
        quint8 note{99};
    };
    std::vector<ChannelNote> m_channelNotes;
};
