#pragma once

#include <QTabWidget>

#include <optional>
#include <vector>

#include "midilearnsetting.h"

namespace midi { struct MidiMessage; }

class MidiTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit MidiTabWidget(QWidget *parent = nullptr);

    MidiLearnSetting learnSetting(int index) const;
    void setLearnSetting(int index, const MidiLearnSetting &learnSetting);

signals:
    void learnSettingChanged(int index, const MidiLearnSetting &learnSetting);

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

    std::vector<MidiLearnSetting> m_learnSettings;
};
