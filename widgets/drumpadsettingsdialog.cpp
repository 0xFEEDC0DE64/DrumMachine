#include "drumpadsettingsdialog.h"
#include "ui_drumpadsettingsdialog.h"

#include <QDebug>
#include <QAbstractButton>

#include "drummachinesettings.h"

DrumPadSettingsDialog::DrumPadSettingsDialog(DrumMachineSettings &settings, QWidget *parent) :
    QDialog{parent},
    m_ui{std::make_unique<Ui::DrumPadSettingsDialog>()},
    m_settings{settings}
{
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, &DrumPadSettingsDialog::buttonClicked);

    resetFields();
}

DrumPadSettingsDialog::~DrumPadSettingsDialog() = default;

void DrumPadSettingsDialog::accept()
{
    if (const quint8 colorPrevPreset = m_ui->spinBoxColorPrevPreset->value();
        m_settings.drumpadColorPrevPreset() != colorPrevPreset)
        m_settings.setDrumpadColorPrevPreset(colorPrevPreset);

    if (const quint8 colorNextPreset = m_ui->spinBoxColorNextPreset->value();
        m_settings.drumpadColorNextPreset() != colorNextPreset)
        m_settings.setDrumpadColorNextPreset(colorNextPreset);

    if (const quint8 colorPrevSequence = m_ui->spinBoxColorPrevSequence->value();
        m_settings.drumpadColorPrevSequence() != colorPrevSequence)
        m_settings.setDrumpadColorPrevSequence(colorPrevSequence);

    if (const quint8 colorNextSequence = m_ui->spinBoxColorNextSequence->value();
        m_settings.drumpadColorNextSequence() != colorNextSequence)
        m_settings.setDrumpadColorNextSequence(colorNextSequence);

    if (const quint8 colorPlayPause = m_ui->spinBoxColorPlayPause->value();
        m_settings.drumpadColorPlayPause() != colorPlayPause)
        m_settings.setDrumpadColorPlayPause(colorPlayPause);

    if (const quint8 colorStopSequence = m_ui->spinBoxColorStopSequence->value();
        m_settings.drumpadColorStopSequence() != colorStopSequence)
        m_settings.setDrumpadColorStopSequence(colorStopSequence);

    if (const quint8 colorSwap = m_ui->spinBoxColorSwap->value();
        m_settings.drumpadColorSwap() != colorSwap)
        m_settings.setDrumpadColorSwap(colorSwap);

    if (const quint8 colorStopAll = m_ui->spinBoxColorStopAll->value();
        m_settings.drumpadColorStopAll() != colorStopAll)
        m_settings.setDrumpadColorStopAll(colorStopAll);

    QDialog::accept();
}

void DrumPadSettingsDialog::buttonClicked(QAbstractButton *button)
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

void DrumPadSettingsDialog::resetFields()
{
    m_ui->spinBoxColorPrevPreset->setValue(m_settings.drumpadColorPrevPreset());
    m_ui->spinBoxColorNextPreset->setValue(m_settings.drumpadColorNextPreset());
    m_ui->spinBoxColorPrevSequence->setValue(m_settings.drumpadColorPrevSequence());
    m_ui->spinBoxColorNextSequence->setValue(m_settings.drumpadColorNextSequence());
    m_ui->spinBoxColorPlayPause->setValue(m_settings.drumpadColorPlayPause());
    m_ui->spinBoxColorStopSequence->setValue(m_settings.drumpadColorStopSequence());
    m_ui->spinBoxColorSwap->setValue(m_settings.drumpadColorSwap());
    m_ui->spinBoxColorStopAll->setValue(m_settings.drumpadColorStopAll());
}

void DrumPadSettingsDialog::restoreDefaults()
{
    m_ui->spinBoxColorPrevPreset->setValue(m_settings.defaultDrumpadColorPrevPreset());
    m_ui->spinBoxColorNextPreset->setValue(m_settings.defaultDrumpadColorNextPreset());
    m_ui->spinBoxColorPrevSequence->setValue(m_settings.defaultDrumpadColorPrevSequence());
    m_ui->spinBoxColorNextSequence->setValue(m_settings.defaultDrumpadColorNextSequence());
    m_ui->spinBoxColorPlayPause->setValue(m_settings.defaultDrumpadColorPlayPause());
    m_ui->spinBoxColorStopSequence->setValue(m_settings.defaultDrumpadColorStopSequence());
    m_ui->spinBoxColorSwap->setValue(m_settings.defaultDrumpadColorSwap());
    m_ui->spinBoxColorStopAll->setValue(m_settings.defaultDrumpadColorStopAll());
}
