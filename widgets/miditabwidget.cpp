#include "miditabwidget.h"

#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "midicontainers.h"

MidiTabWidget::MidiTabWidget(QWidget *parent) :
    QTabWidget{parent}
{
    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar(), &QWidget::customContextMenuRequested, this, &MidiTabWidget::showContextMenu);
}

quint8 MidiTabWidget::channel(int index) const
{
    return m_channelNotes[index].channel;
}

void MidiTabWidget::setChannel(int index, quint8 channel)
{
    auto &channelNote = m_channelNotes[index];
    if (channelNote.channel == channel)
        return;
    emit channelChanged(index, channelNote.channel = channel);
}

quint8 MidiTabWidget::note(int index) const
{
    return m_channelNotes[index].note;
}

void MidiTabWidget::setNote(int index, quint8 note)
{
    auto &channelNote = m_channelNotes[index];
    if (channelNote.note == note)
        return;
    emit noteChanged(index, channelNote.note = note);
}

void MidiTabWidget::learn(int index)
{
    if (m_learning)
    {
        tabBar()->setTabTextColor(*m_learning, m_oldColor);

        if (*m_learning == index)
            m_learning = std::nullopt;
        else
            goto startLearn;
    }
    else
    {
startLearn:
        m_oldColor = tabBar()->tabTextColor(index);
        tabBar()->setTabTextColor(index, Qt::red);
        m_learning = index;
    }
}

void MidiTabWidget::midiReceived(const midi::MidiMessage &message)
{
    if ((message.cmd != midi::Command::NoteOn && message.cmd != midi::Command::ControlChange) || message.velocity == 0)
        return;

    if (m_learning)
    {
        qDebug() << "learning" << message.cmd << message.velocity << message.channel << message.note;
        setChannel(*m_learning, message.channel);
        setNote(*m_learning, message.note);
        learn(*m_learning);
    }
    else
    {
        qDebug() << "normal" << message.cmd << message.velocity << message.channel << message.note;
        for (int i = 0; i < count(); i++)
        {
            if (message.channel != m_channelNotes[i].channel ||
                message.note != m_channelNotes[i].note)
                continue;

            setCurrentIndex(i);
            return;
        }
    }
}

void MidiTabWidget::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
    m_channelNotes.insert(std::begin(m_channelNotes) + index, ChannelNote{.channel=quint8(index)});
}

void MidiTabWidget::tabRemoved(int index)
{
    QTabWidget::tabInserted(index);
    m_channelNotes.erase(std::begin(m_channelNotes) + index);
}

void MidiTabWidget::showContextMenu(const QPoint &pos)
{
    const auto index = tabBar()->tabAt(pos);
    if (index < 0)
        return;

    QMenu menu{tabBar()};
    const auto learnAction = menu.addAction(tr("Learn..."));
    if (const auto selectedAction = menu.exec(tabBar()->mapToGlobal(pos));
            selectedAction == learnAction)
    {
        learn(index);
    }
}
