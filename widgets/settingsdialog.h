#pragma once

#include <QDialog>

#include <memory>

namespace Ui { class SettingsDialog; }
class DrumMachineSettings;
class QAbstractButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(DrumMachineSettings &settings, qint64 currentCacheSize, QWidget *parent = nullptr);
    ~SettingsDialog() override;

public slots:
    void accept() override;

private slots:
    void buttonClicked(QAbstractButton *button);
    void resetFields();
    void restoreDefaults();
    void selectCacheDir();

private:
    const std::unique_ptr<Ui::SettingsDialog> m_ui;
    DrumMachineSettings &m_settings;
};
