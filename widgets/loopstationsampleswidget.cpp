#include "loopstationsampleswidget.h"
#include "ui_loopstationsampleswidget.h"

LoopStationSamplesWidget::LoopStationSamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::LoopStationSamplesWidget>()}
{
    m_ui->setupUi(this);
}

LoopStationSamplesWidget::~LoopStationSamplesWidget() = default;

void LoopStationSamplesWidget::loadSettings(DrumMachineSettings &settings)
{
    Q_UNUSED(settings)
}

void LoopStationSamplesWidget::setPreset(const loopstation_presets::Preset &preset)
{
    Q_UNUSED(preset)
}

void LoopStationSamplesWidget::midiReceived(const midi::MidiMessage &message)
{
    Q_UNUSED(message)
}

void LoopStationSamplesWidget::writeSamples(frame_t *begin, frame_t *end)
{
    Q_UNUSED(begin)
    Q_UNUSED(end)
}

void LoopStationSamplesWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    Q_UNUSED(networkAccessManager)
}

void LoopStationSamplesWidget::injectDecodingThread(QThread &thread)
{
    Q_UNUSED(thread)
}

void LoopStationSamplesWidget::unsendColors()
{

}

void LoopStationSamplesWidget::sendColors()
{

}

std::array<std::reference_wrapper<LoopStationSampleWidget>, 48> LoopStationSamplesWidget::getWidgets()
{
    return {
        std::ref(*m_ui->sampleWidget0),
        std::ref(*m_ui->sampleWidget1),
        std::ref(*m_ui->sampleWidget2),
        std::ref(*m_ui->sampleWidget3),
        std::ref(*m_ui->sampleWidget4),
        std::ref(*m_ui->sampleWidget5),
        std::ref(*m_ui->sampleWidget6),
        std::ref(*m_ui->sampleWidget7),
        std::ref(*m_ui->sampleWidget8),
        std::ref(*m_ui->sampleWidget9),
        std::ref(*m_ui->sampleWidget10),
        std::ref(*m_ui->sampleWidget11),
        std::ref(*m_ui->sampleWidget12),
        std::ref(*m_ui->sampleWidget13),
        std::ref(*m_ui->sampleWidget14),
        std::ref(*m_ui->sampleWidget15),
        std::ref(*m_ui->sampleWidget16),
        std::ref(*m_ui->sampleWidget17),
        std::ref(*m_ui->sampleWidget18),
        std::ref(*m_ui->sampleWidget19),
        std::ref(*m_ui->sampleWidget20),
        std::ref(*m_ui->sampleWidget21),
        std::ref(*m_ui->sampleWidget22),
        std::ref(*m_ui->sampleWidget23),
        std::ref(*m_ui->sampleWidget24),
        std::ref(*m_ui->sampleWidget25),
        std::ref(*m_ui->sampleWidget26),
        std::ref(*m_ui->sampleWidget27),
        std::ref(*m_ui->sampleWidget28),
        std::ref(*m_ui->sampleWidget29),
        std::ref(*m_ui->sampleWidget30),
        std::ref(*m_ui->sampleWidget31),
        std::ref(*m_ui->sampleWidget32),
        std::ref(*m_ui->sampleWidget33),
        std::ref(*m_ui->sampleWidget34),
        std::ref(*m_ui->sampleWidget35),
        std::ref(*m_ui->sampleWidget36),
        std::ref(*m_ui->sampleWidget37),
        std::ref(*m_ui->sampleWidget38),
        std::ref(*m_ui->sampleWidget39),
        std::ref(*m_ui->sampleWidget40),
        std::ref(*m_ui->sampleWidget41),
        std::ref(*m_ui->sampleWidget42),
        std::ref(*m_ui->sampleWidget43),
        std::ref(*m_ui->sampleWidget44),
        std::ref(*m_ui->sampleWidget45),
        std::ref(*m_ui->sampleWidget46),
        std::ref(*m_ui->sampleWidget47),
    };
}
