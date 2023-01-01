#pragma once

#include <QDialog>

#include <memory>

namespace Ui { class LoopStationSettingsDialog; }
class DrumMachineSettings;
class QAbstractButton;

class LoopStationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoopStationSettingsDialog(DrumMachineSettings &settings, QWidget *parent = nullptr);
    ~LoopStationSettingsDialog() override;

public slots:
    void accept() override;

private slots:
    void buttonClicked(QAbstractButton *button);
    void resetFields();
    void restoreDefaults();

private:
    const std::unique_ptr<Ui::LoopStationSettingsDialog> m_ui;

    DrumMachineSettings &m_settings;
};
