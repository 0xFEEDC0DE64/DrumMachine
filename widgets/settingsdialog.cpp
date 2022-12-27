#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

#include "drummachinesettings.h"

SettingsDialog::SettingsDialog(DrumMachineSettings &settings, qint64 currentCacheSize, QWidget *parent) :
    QDialog{parent},
    m_ui{std::make_unique<Ui::SettingsDialog>()},
    m_settings{settings}
{
    m_ui->setupUi(this);

    connect(m_ui->toolButtonSelectCacheDir, &QAbstractButton::pressed, this, &SettingsDialog::selectCacheDir);
    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingsDialog::buttonClicked);

    resetFields();

    m_ui->labelCurrentCacheSize->setText(tr("(Current Cache Size: %0MB)").arg(currentCacheSize / 1024 / 1024));
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::accept()
{
    {
        const auto &cacheDir = m_ui->lineEditCacheDir->text();

        try
        {
            if (cacheDir.isEmpty())
                throw tr("Empty cacheDir!");
            else if (QDir dir{cacheDir}; !dir.exists() && !dir.mkpath(dir.absolutePath()))
                throw tr("Could not create cache directory!");
        }
        catch (const QString &e)
        {
            QMessageBox::warning(this, e, e);
            return;
        }

        if (m_settings.cacheDir() != cacheDir)
            m_settings.setCacheDir(cacheDir);
    }

    const auto maximumCacheSize = qint64{m_ui->spinBoxMaxCacheSize->value()} * 1024 * 1024;
    if (m_settings.maximumCacheSize() != maximumCacheSize)
        m_settings.setMaximumCacheSize(maximumCacheSize);

    QDialog::accept();
}

void SettingsDialog::buttonClicked(QAbstractButton *button)
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

void SettingsDialog::resetFields()
{
    m_ui->lineEditCacheDir->setText(m_settings.cacheDir());
    m_ui->spinBoxMaxCacheSize->setValue(m_settings.maximumCacheSize() / 1024 / 1024);
}

void SettingsDialog::restoreDefaults()
{
    m_ui->lineEditCacheDir->setText(m_settings.defaultCacheDir());
    m_ui->spinBoxMaxCacheSize->setValue(m_settings.defaultMaximumCacheSize() / 1024 / 1024);
}

void SettingsDialog::selectCacheDir()
{
    QDir dir{m_ui->lineEditCacheDir->text()};
    if (!dir.exists())
        dir = QDir{m_settings.cacheDir()};
    const auto selected = QFileDialog::getExistingDirectory(this, tr("Select cache dir"), dir.exists() ? dir.absolutePath() : QString{});
    if (selected.isEmpty())
        return;
    m_ui->lineEditCacheDir->setText(selected);
}
