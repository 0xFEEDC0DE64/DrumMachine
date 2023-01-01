#pragma once

#include <QDialog>

#include <memory>

namespace Ui { class DrumPadSettingsDialog; }
class DrumMachineSettings;
class QAbstractButton;

class DrumPadSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DrumPadSettingsDialog(DrumMachineSettings &settings, QWidget *parent = nullptr);
    ~DrumPadSettingsDialog() override;

public slots:
    void accept() override;

private slots:
    void buttonClicked(QAbstractButton *button);
    void resetFields();
    void restoreDefaults();

private:
    const std::unique_ptr<Ui::DrumPadSettingsDialog> m_ui;

    DrumMachineSettings &m_settings;
};
