#include "loopstationsampleswidget.h"
#include "ui_loopstationsampleswidget.h"

#include <algorithm>

#include "loopstationpresets.h"
#include "drummachinesettings.h"

LoopStationSamplesWidget::LoopStationSamplesWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::LoopStationSamplesWidget>()},
    m_timer{this}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonSettings, &QAbstractButton::pressed, this, &LoopStationSamplesWidget::showSettings);

    connect(m_ui->spinBoxBpm, qOverload<int>(&QSpinBox::valueChanged), this, &LoopStationSamplesWidget::tempoChanged);

    connect(m_ui->pushButtonPlayPause, &QAbstractButton::pressed, this, &LoopStationSamplesWidget::playPausePressed);
    connect(m_ui->pushButtonStop, &QAbstractButton::pressed, this, &LoopStationSamplesWidget::stopPressed);

    connect(&m_timer, &QTimer::timeout, this, &LoopStationSamplesWidget::timeout);

    m_timer.setTimerType(Qt::PreciseTimer);

    quint8 padNr{};
    for (LoopStationSampleWidget &widget : getWidgets())
    {
        widget.setPadNr(padNr++);
        connect(&widget, &LoopStationSampleWidget::sendMidi, this, &LoopStationSamplesWidget::sendMidi);
        connect(&widget, &LoopStationSampleWidget::loopEnabledChanged, this, &LoopStationSamplesWidget::loopEnabledChanged);
    }

    constexpr const auto setCategories = [](auto category, auto *widget0, auto *widget1, auto *widget2, auto *widget3, auto *widget4, auto *widget5, auto *widget6, auto *widget7){
        widget0->setCategory(category);
        widget1->setCategory(category);
        widget2->setCategory(category);
        widget3->setCategory(category);
        widget4->setCategory(category);
        widget5->setCategory(category);
        widget6->setCategory(category);
        widget7->setCategory(category);
    };
    setCategories(0, m_ui->sampleWidget0,  m_ui->sampleWidget1,  m_ui->sampleWidget2,  m_ui->sampleWidget3,
                     m_ui->sampleWidget24, m_ui->sampleWidget25, m_ui->sampleWidget26, m_ui->sampleWidget27);
    setCategories(1, m_ui->sampleWidget4,  m_ui->sampleWidget5,  m_ui->sampleWidget6,  m_ui->sampleWidget7,
                     m_ui->sampleWidget28, m_ui->sampleWidget29, m_ui->sampleWidget30, m_ui->sampleWidget31);
    setCategories(2, m_ui->sampleWidget8,  m_ui->sampleWidget9,  m_ui->sampleWidget10, m_ui->sampleWidget11,
                     m_ui->sampleWidget32, m_ui->sampleWidget33, m_ui->sampleWidget34, m_ui->sampleWidget35);
    setCategories(3, m_ui->sampleWidget12, m_ui->sampleWidget13, m_ui->sampleWidget14, m_ui->sampleWidget15,
                     m_ui->sampleWidget36, m_ui->sampleWidget37, m_ui->sampleWidget38, m_ui->sampleWidget39);
    setCategories(4, m_ui->sampleWidget16, m_ui->sampleWidget17, m_ui->sampleWidget18, m_ui->sampleWidget19,
                     m_ui->sampleWidget40, m_ui->sampleWidget41, m_ui->sampleWidget42, m_ui->sampleWidget43);
    setCategories(5, m_ui->sampleWidget20, m_ui->sampleWidget21, m_ui->sampleWidget22, m_ui->sampleWidget23,
                     m_ui->sampleWidget44, m_ui->sampleWidget45, m_ui->sampleWidget46, m_ui->sampleWidget47);
}

LoopStationSamplesWidget::~LoopStationSamplesWidget() = default;

void LoopStationSamplesWidget::loadSettings(DrumMachineSettings &settings)
{
    m_settings = &settings;

    m_ui->pushButtonPlayPause->setLearnSetting(settings.loopstationPlayPause());
    m_ui->pushButtonStop->setLearnSetting(settings.loopstationStop());

    connect(m_ui->pushButtonPlayPause, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setLoopstationPlayPause);
    connect(m_ui->pushButtonStop, &MidiButton::learnSettingChanged, &settings, &DrumMachineSettings::setLoopstationStop);

    for (LoopStationSampleWidget &widget : getWidgets())
        widget.loadSettings(settings);
}

void LoopStationSamplesWidget::setPreset(const loopstation_presets::Preset &preset)
{
    assert(preset.id);
    assert(preset.pads);
    assert(preset.bpm);

    const auto &presetId = *preset.id;
    const auto &widgets = getWidgets();
    const auto &pads = *preset.pads;

    stopPressed();
    m_ui->spinBoxBpm->setValue(*preset.bpm);

    auto iter = std::begin(widgets);
    auto iter2 = std::begin(pads);
    int i{};
    for (; iter != std::end(widgets) && iter2 != std::end(pads); iter++, iter2++)
    {
        auto &widget = ((*iter)).get();
        widget.setSample(presetId, QString{"%0_%1.wav"}.arg(presetId).arg(++i, 2, 10, QLatin1Char('0')), *iter2);
    }
}

void LoopStationSamplesWidget::midiReceived(const midi::MidiMessage &message)
{
    m_ui->pushButtonPlayPause->midiReceived(message);
    m_ui->pushButtonStop->midiReceived(message);

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
    const quint8 color = m_settings ? m_settings->colorOff() : quint8{0};

    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlayPause->learnSetting().channel,
        .cmd = m_ui->pushButtonPlayPause->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlayPause->learnSetting().note,
        .velocity = color
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStop->learnSetting().channel,
        .cmd = m_ui->pushButtonStop->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStop->learnSetting().note,
        .velocity = color
    });

    for (LoopStationSampleWidget &widget : getWidgets())
        widget.unsendColor();
}

void LoopStationSamplesWidget::sendColors()
{
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonPlayPause->learnSetting().channel,
        .cmd = m_ui->pushButtonPlayPause->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonPlayPause->learnSetting().note,
        .velocity = 3
    });
    emit sendMidi(midi::MidiMessage {
        .channel = m_ui->pushButtonStop->learnSetting().channel,
        .cmd = m_ui->pushButtonStop->learnSetting().cmd,
        .flag = true,
        .note = m_ui->pushButtonStop->learnSetting().note,
        .velocity = 60
    });

    for (LoopStationSampleWidget &widget : getWidgets())
        widget.sendColor(true);
}

void LoopStationSamplesWidget::showSettings()
{

}

void LoopStationSamplesWidget::timeout()
{
    if (m_pos >= 7)
    {
        for (LoopStationSampleWidget &widget : getWidgets())
            widget.timeout();
        m_pos = 0;
    }
    else
    {
        if (m_pos == 3)
        {
            for (LoopStationSampleWidget &widget : getWidgets())
                widget.timeout();
        }

        m_pos++;
    }
    m_ui->horizontalSlider->setValue(m_pos);
}

void LoopStationSamplesWidget::tempoChanged(int tempo)
{
    m_timer.setInterval(1000. * 60. / tempo);
}

void LoopStationSamplesWidget::loopEnabledChanged(bool loopEnabled, quint8 category)
{
    if (loopEnabled)
    {
        for (LoopStationSampleWidget &widget : getWidgets())
        {
            if (widget.category() != category)
                continue;
            if (&widget == sender())
                continue;
            widget.setLoopEnabled(false);
        }

        if (!m_timer.isActive())
        {
            m_pos = 0;
            m_ui->horizontalSlider->setValue(m_pos);
            m_timer.start();
            m_ui->pushButtonPlayPause->setText(tr("▮▮"));
            for (LoopStationSampleWidget &widget : getWidgets())
                widget.timeout();
        }
    }
    else
    {
        const auto widgets = getWidgets();
        if (m_timer.isActive() && std::none_of(std::begin(widgets), std::end(widgets), [](const LoopStationSampleWidget &widget){
                                      return widget.loopEnabled();
                                  }))
        {
            m_timer.stop();
            m_ui->pushButtonPlayPause->setText(tr("▶"));
            m_pos = 0;
            m_ui->horizontalSlider->setValue(m_pos);
        }
    }
}

void LoopStationSamplesWidget::playPausePressed()
{
    if (m_timer.isActive())
    {
        m_timer.stop();
        m_ui->pushButtonPlayPause->setText(tr("▶"));
    }
    else
    {
        m_timer.start();
        m_ui->pushButtonPlayPause->setText(tr("▮▮"));
        if (m_pos == 0)
            for (LoopStationSampleWidget &widget : getWidgets())
                widget.timeout();
    }
}

void LoopStationSamplesWidget::stopPressed()
{
    m_timer.stop();
    m_ui->pushButtonPlayPause->setText(tr("▶"));
    for (LoopStationSampleWidget &widget : getWidgets())
    {
        widget.setLoopEnabled(false);
        widget.stop();
    }
    m_pos = 0;
    m_ui->horizontalSlider->setValue(m_pos);
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
