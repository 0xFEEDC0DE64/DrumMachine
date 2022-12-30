#include "midibutton.h"

#include <QAction>
#include <QtGlobal>

#include "midicontainers.h"
#include "futurecpp.h"

MidiButton::MidiButton(QWidget *parent) :
    QPushButton{parent}
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    auto action = new QAction{tr("Learn...")};
    connect(action, &QAction::triggered, this, &MidiButton::learn);
    addAction(action);

    {
        m_actionCmd = new QAction{tr("cmd:")};
        m_actionCmd->setDisabled(true);
        addAction(m_actionCmd);
    }
    {
        m_actionChannel = new QAction{tr("channel:")};
        m_actionChannel->setDisabled(true);
        addAction(m_actionChannel);
    }
    {
        m_actionNote = new QAction{tr("note:")};
        m_actionNote->setDisabled(true);
        addAction(m_actionNote);
    }
}

MidiButton::MidiButton(const QString &text, QWidget *parent) :
    QPushButton{text, parent}
{
}

MidiButton::MidiButton(const QIcon &icon, const QString &text, QWidget *parent) :
    QPushButton{icon, text, parent}
{
}

void MidiButton::setLearnSetting(const MidiLearnSetting &learnSetting)
{
    if (m_learnSetting == learnSetting)
        return;
    emit learnSettingChanged(m_learnSetting = learnSetting);
    m_actionCmd->setText(tr("cmd: %0").arg(std::to_underlying(m_learnSetting.cmd)));
    m_actionChannel->setText(tr("channel: %0").arg(m_learnSetting.channel));
    m_actionNote->setText(tr("note: %0").arg(m_learnSetting.note));
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
    if (m_learning)
    {
        if ((message.cmd != midi::Command::NoteOn && message.cmd != midi::Command::ControlChange) || message.velocity == 0)
            return;

        setLearnSetting(MidiLearnSetting{
                            .cmd = message.cmd == midi::Command::NoteOff ? midi::Command::NoteOn : message.cmd,
                            .channel = message.channel,
                            .note = message.note
                        });

        learn();
    }
    else
    {
        if (message.channel != m_learnSetting.channel ||
            message.note != m_learnSetting.note)
            return;

        if (m_learnSetting.cmd == midi::Command::NoteOn || m_learnSetting.cmd == midi::Command::NoteOff)
        {
            switch (message.cmd)
            {
            case midi::Command::NoteOn:
                if (message.velocity != 0)
                {
                    if (isCheckable())
                        toggle();
                    emit pressed();
                }
                else
            Q_FALLTHROUGH();
            case midi::Command::NoteOff:
                emit released();
                break;
            default:
                return;
            }
        }
        else if (m_learnSetting.cmd == message.cmd)
        {
            if (message.velocity != 0)
            {
                if (isCheckable())
                    toggle();
                emit pressed();
            }
            else
                emit released();
        }
        else
            return;
    }
}
