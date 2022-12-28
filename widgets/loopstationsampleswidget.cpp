#include "loopstationsampleswidget.h"
#include "ui_loopstationsampleswidget.h"

#include "loopstationpresets.h"

LoopStationSamplesWidget::LoopStationSamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::LoopStationSamplesWidget>()}
{
    m_ui->setupUi(this);

    quint8 padNr{};
    for (LoopStationSampleWidget &widget : getWidgets())
    {
        widget.setPadNr(padNr++);
        connect(&widget, &LoopStationSampleWidget::sendMidi, this, &LoopStationSamplesWidget::sendMidi);
    }
}

LoopStationSamplesWidget::~LoopStationSamplesWidget() = default;

void LoopStationSamplesWidget::loadSettings(DrumMachineSettings &settings)
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.loadSettings(settings);
}

void LoopStationSamplesWidget::setPreset(const loopstation_presets::Preset &preset)
{
    assert(preset.id);
    assert(preset.pads);

    const auto &presetId = *preset.id;
    const auto &widgets = getWidgets();
    const auto &pads = *preset.pads;

    auto iter = std::begin(widgets);
    auto iter2 = std::begin(pads);
    int i{};
    for (; iter != std::end(widgets) && iter2 != std::end(pads); iter++, iter2++)
    {
        ((*iter)).get().setSample(presetId, QString{"%0_%1.wav"}.arg(presetId).arg(++i, 2, 10, QLatin1Char('0')), *iter2);
    }
}

void LoopStationSamplesWidget::midiReceived(const midi::MidiMessage &message)
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.midiReceived(message);
}

void LoopStationSamplesWidget::writeSamples(frame_t *begin, frame_t *end)
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.writeSamples(begin, end);
}

void LoopStationSamplesWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.injectNetworkAccessManager(networkAccessManager);
}

void LoopStationSamplesWidget::injectDecodingThread(QThread &thread)
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.injectDecodingThread(thread);
}

void LoopStationSamplesWidget::unsendColors()
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.unsendColor();
}

void LoopStationSamplesWidget::sendColors()
{
    for (LoopStationSampleWidget &widget : getWidgets())
        widget.sendColor();
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
