#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaEnum>
#include <QMessageBox>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QAudioDeviceInfo>

#include "presets.h"
#include "midiinwrapper.h"
#include "midicontainers.h"

MainWindow::MainWindow(const presets::PresetsConfig &presetsConfig, QWidget *parent) :
    QMainWindow{parent},
    m_ui{std::make_unique<Ui::MainWindow>()},
    m_presetsModel{*presetsConfig.presets}
{
    m_ui->setupUi(this);

    connect(&m_midiIn, &MidiInWrapper::messageReceived, this, &MainWindow::messageReceived);

    updateMidiDevices();

    connect(m_ui->pushButtonMidiController, &QAbstractButton::pressed, this, [this](){
        if (m_midiIn.isPortOpen())
            m_midiIn.closePort();
        else
        {
            const auto index = m_ui->comboBoxMidiController->currentIndex();
            if (index != -1)
                m_midiIn.openPort(index);
        }

        m_ui->pushButtonMidiController->setText(m_midiIn.isPortOpen() ? tr("Close") : tr("Open"));
    });

    updateAudioDevices();

    {
        const auto index = m_devices.indexOf(QAudioDeviceInfo::defaultOutputDevice());
        if (index != -1)
            m_ui->comboBoxAudioDevice->setCurrentIndex(index);
    }

    {
        const auto callback = [this](int index){
            m_ui->samplesWidget->setAudioDevice(m_devices.at(index));
        };
        connect(m_ui->comboBoxAudioDevice, qOverload<int>(&QComboBox::currentIndexChanged), m_ui->samplesWidget, callback);
        callback(m_ui->comboBoxAudioDevice->currentIndex());
    }

    m_presetsProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_presetsProxyModel.setSourceModel(&m_presetsModel);
    m_ui->presetsView->setModel(&m_presetsProxyModel);

    m_presetsProxyModel.setFilterKeyColumn(1);

    connect(m_ui->lineEdit, &QLineEdit::textChanged, this, [=](){
        m_presetsProxyModel.setFilterFixedString(m_ui->lineEdit->text());
    });

    m_ui->filesView->setModel(&m_filesModel);

    connect(m_ui->presetsView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::currentRowChanged);
}

MainWindow::~MainWindow() = default;

void MainWindow::selectFirstPreset()
{
    if (m_presetsProxyModel.rowCount())
    {
        const auto index = m_presetsProxyModel.index(0, 0);
        if (index.isValid())
        {
            m_ui->presetsView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            currentRowChanged(index);
        }
    }
}

void MainWindow::messageReceived(const midi::MidiMessage &message)
{
    m_ui->statusbar->showMessage(tr("Received midi message: flag: %0 cmd: %1 channel: %2 note: %3 velocity: %4")
                                 .arg(message.flag?"true":"false", QMetaEnum::fromType<midi::Command>().valueToKey(int(message.cmd)))
                                 .arg(message.channel).arg(message.note).arg(message.velocity), 1000);

    m_ui->samplesWidget->messageReceived(message);
}

void MainWindow::currentRowChanged(const QModelIndex &current)
{
    if (!current.isValid())
        return;

    const auto &preset = m_presetsModel.getPreset(m_presetsProxyModel.mapToSource(current));

    m_ui->presetDetailWidget->setPreset(preset);
    m_filesModel.setPreset(preset);
    m_ui->samplesWidget->setPreset(preset);
}

void MainWindow::updateMidiDevices()
{
    m_ui->comboBoxMidiController->clear();

    for (const auto &name : m_midiIn.portNames())
        m_ui->comboBoxMidiController->addItem(name);

    m_ui->pushButtonMidiController->setEnabled(m_ui->comboBoxMidiController->count() > 0);
}

void MainWindow::updateAudioDevices()
{
    m_ui->comboBoxAudioDevice->clear();

    m_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    for (const auto &device : m_devices)
        m_ui->comboBoxAudioDevice->addItem(device.deviceName());
}
