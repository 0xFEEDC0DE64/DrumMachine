#pragma once

#include <QFrame>

#include <memory>

namespace Ui { class LoopStationSampleWidget; }

class LoopStationSampleWidget : public QFrame
{
    Q_OBJECT

public:
    explicit LoopStationSampleWidget(QWidget *parent = nullptr);
    ~LoopStationSampleWidget();

private:
    const std::unique_ptr<Ui::LoopStationSampleWidget> m_ui;
};
