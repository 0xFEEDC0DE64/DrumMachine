#include "loopstationsettingsdialog.h"
#include "ui_loopstationsettingsdialog.h"

#include <QDebug>
#include <QAbstractButton>

#include "drummachinesettings.h"

LoopStationSettingsDialog::LoopStationSettingsDialog(DrumMachineSettings &settings, QWidget *parent) :
    QDialog{parent},
    m_ui{std::make_unique<Ui::LoopStationSettingsDialog>()},
    m_settings{settings}
{
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, &LoopStationSettingsDialog::buttonClicked);

    resetFields();
}

LoopStationSettingsDialog::~LoopStationSettingsDialog() = default;

void LoopStationSettingsDialog::accept()
{
    if (const quint8 colorPrevPreset = m_ui->spinBoxColorPrevPreset->value();
        m_settings.loopstationColorPrevPreset() != colorPrevPreset)
        m_settings.setLoopstationColorPrevPreset(colorPrevPreset);

    if (const quint8 colorNextPreset = m_ui->spinBoxColorNextPreset->value();
        m_settings.loopstationColorNextPreset() != colorNextPreset)
        m_settings.setLoopstationColorNextPreset(colorNextPreset);

    if (const quint8 colorPlayPause = m_ui->spinBoxColorPlayPause->value();
        m_settings.loopstationColorPlayPause() != colorPlayPause)
        m_settings.setLoopstationColorPlayPause(colorPlayPause);

    if (const quint8 colorStop = m_ui->spinBoxColorStop->value();
        m_settings.loopstationColorStop() != colorStop)
        m_settings.setLoopstationColorStop(colorStop);

    QDialog::accept();
}

void LoopStationSettingsDialog::buttonClicked(QAbstractButton *button)
{
    if (!button)
    {
        qWarning() << "invalid button";
        return;
    }

    switch (m_ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Reset:
        resetFields();
        return;
    case QDialogButtonBox::RestoreDefaults:
        restoreDefaults();
        return;
    default:;
    }
}

void LoopStationSettingsDialog::resetFields()
{
    m_ui->spinBoxColorPrevPreset->setValue(m_settings.loopstationColorPrevPreset());
    m_ui->spinBoxColorNextPreset->setValue(m_settings.loopstationColorNextPreset());
    m_ui->spinBoxColorPlayPause->setValue(m_settings.loopstationColorPlayPause());
    m_ui->spinBoxColorStop->setValue(m_settings.loopstationColorStop());
}

void LoopStationSettingsDialog::restoreDefaults()
{
    m_ui->spinBoxColorPrevPreset->setValue(m_settings.defaultLoopstationColorPrevPreset());
    m_ui->spinBoxColorNextPreset->setValue(m_settings.defaultLoopstationColorNextPreset());
    m_ui->spinBoxColorPlayPause->setValue(m_settings.defaultLoopstationColorPlayPause());
    m_ui->spinBoxColorStop->setValue(m_settings.defaultLoopstationColorStop());
}
