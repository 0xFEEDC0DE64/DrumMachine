#include "loopstationwidget.h"
#include "ui_loopstationwidget.h"

#include "audioformat.h"

LoopStationWidget::LoopStationWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::LoopStationWidget>()}
{
    m_ui->setupUi(this);
}

LoopStationWidget::~LoopStationWidget() = default;

void LoopStationWidget::writeSamples(frame_t *begin, frame_t *end)
{

}

void LoopStationWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{

}

void LoopStationWidget::injectDecodingThread(QThread &thread)
{

}

void LoopStationWidget::loadSettings(DrumMachineSettings &settings)
{

}

void LoopStationWidget::unsendColors()
{

}

void LoopStationWidget::sendColors()
{

}

void LoopStationWidget::midiReceived(const midi::MidiMessage &message)
{

}
