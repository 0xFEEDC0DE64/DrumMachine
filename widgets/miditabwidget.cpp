#include "miditabwidget.h"

#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "midicontainers.h"
#include "futurecpp.h"

MidiTabWidget::MidiTabWidget(QWidget *parent) :
    QTabWidget{parent}
{
    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar(), &QWidget::customContextMenuRequested, this, &MidiTabWidget::showContextMenu);
}

MidiLearnSetting MidiTabWidget::learnSetting(int index) const
{
    return m_learnSettings[index];
}

void MidiTabWidget::setLearnSetting(int index, const MidiLearnSetting &learnSetting)
{
    if (learnSetting == m_learnSettings[index])
        return;
    emit learnSettingChanged(index, m_learnSettings[index] = learnSetting);
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
        setLearnSetting(*m_learning, MidiLearnSetting{
                            .cmd = message.cmd == midi::Command::NoteOff ? midi::Command::NoteOn : message.cmd,
                            .channel = message.channel,
                            .note = message.note
                        });

        learn(*m_learning);
    }
    else
    {
        for (int i = 0; i < count(); i++)
        {
            if (message.cmd != m_learnSettings[i].cmd ||
                message.channel != m_learnSettings[i].channel ||
                message.note != m_learnSettings[i].note)
                continue;

            setCurrentIndex(i);
            return;
        }
    }
}

void MidiTabWidget::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
    m_learnSettings.insert(std::begin(m_learnSettings) + index, MidiLearnSetting{});
}

void MidiTabWidget::tabRemoved(int index)
{
    QTabWidget::tabInserted(index);
    m_learnSettings.erase(std::begin(m_learnSettings) + index);
}

void MidiTabWidget::showContextMenu(const QPoint &pos)
{
    const auto index = tabBar()->tabAt(pos);
    if (index < 0)
        return;

    QMenu menu{tabBar()};
    const auto learnAction = menu.addAction(tr("Learn..."));
    {
        const auto &learnSetting = m_learnSettings[index];
        menu.addAction(tr("cmd: %0").arg(std::to_underlying(learnSetting.cmd)))->setDisabled(true);
        menu.addAction(tr("channel: %0").arg(learnSetting.channel))->setDisabled(true);
        menu.addAction(tr("note: %0").arg(learnSetting.note))->setDisabled(true);
    }
    if (const auto selectedAction = menu.exec(tabBar()->mapToGlobal(pos));
            selectedAction == learnAction)
    {
        learn(index);
    }
}
