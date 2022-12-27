#include "midibutton.h"

#include <QAction>

#include "midicontainers.h"

MidiButton::MidiButton(QWidget *parent) :
    QPushButton{parent}
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    auto action = new QAction{tr("Learn...")};
    connect(action, &QAction::triggered, this, &MidiButton::learn);
    addAction(action);
}

MidiButton::MidiButton(const QString &text, QWidget *parent) :
    QPushButton{text, parent}
{
}

MidiButton::MidiButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton{icon, text, parent}
{
}

void MidiButton::learn()
{
    auto palette = this->palette();

    if (m_learning)
    {
        palette.setColor(backgroundRole(), m_oldColor);
        palette.setBrush(backgroundRole(), m_oldBrush);
    }
    else
    {
        m_oldColor = palette.color(backgroundRole());
        m_oldBrush = palette.brush(backgroundRole());
        palette.setColor(backgroundRole(), Qt::red);
        palette.setBrush(backgroundRole(), Qt::red);
    }
    setPalette(palette);

    m_learning = !m_learning;
}

void MidiButton::midiReceived(const midi::MidiMessage &message)
{
    if (message.cmd != midi::Command::NoteOn && message.cmd != midi::Command::NoteOff)
        return;

    if (m_learning)
    {
        if (message.cmd != midi::Command::NoteOn)
            return;

        setChannel(message.channel);
        setNote(message.note);
        learn();
    }
    else
    {
        if (message.channel != m_channel || message.note != m_note)
            return;

        switch (message.cmd)
        {
        case midi::Command::NoteOn:
            if (message.velocity == 0)
                emit released();
            else
                emit pressed();
            return;
        case midi::Command::NoteOff:
            emit released();
            return;
        default: __builtin_unreachable();
        }
    }
}