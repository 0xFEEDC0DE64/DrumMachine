#include "samplewidget.h"
#include "ui_samplewidget.h"

#include <QAbstractEventDispatcher>
#include <QAudioDeviceInfo>
#include <QSoundEffect>
#include <QDebug>

namespace {
QString toString(QString value) { return value; }
QString toString(int value) { return QString::number(value); }
QString toString(bool value) { return value?"true":"false"; }
QString toString(QSoundEffect::Status value)
{
    switch (value)
    {
    case QSoundEffect::Null: return "Null";
    case QSoundEffect::Loading: return "Loading";
    case QSoundEffect::Ready: return "Ready";
    case QSoundEffect::Error: return "Error";
    }

    return QString{"Unknown (%0)"}.arg(value);
}
}

SampleWidget::SampleWidget(QWidget *parent) :
    QFrame{parent},
    m_ui{std::make_unique<Ui::SampleWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButton, &QAbstractButton::pressed, this, [this](){ pressed(127); });
    connect(m_ui->pushButton, &QAbstractButton::released, this, &SampleWidget::released);

    updateStatus();
}

SampleWidget::~SampleWidget()
{
    if (m_effect)
        QMetaObject::invokeMethod(m_effect.get(), [effect=m_effect.release()](){ delete effect; });
}

void SampleWidget::setFile(const QString &presetId, const presets::File &file)
{
    m_presetId = presetId;
    m_file = file;

    if (m_effect)
    {
        auto sampleUrl = this->sampleUrl();
        if (!sampleUrl.isEmpty())
            QMetaObject::invokeMethod(m_effect.get(), [&effect=*m_effect,sampleUrl=std::move(sampleUrl)](){
                effect.setSource(sampleUrl);
            });
    }

    const auto setupLabel = [&](const auto &value, QLabel *label){
        QString text;
        QFont font;
        QPalette pal;

        if (value)
            text = toString(*value);
        else
        {
            text = tr("(null)");
            font.setItalic(true);
            pal.setColor(label->foregroundRole(), Qt::gray);
        }

        label->setText(text);
        label->setFont(font);
        label->setPalette(pal);
    };

    setupLabel(file.stopOnRelease, m_ui->stopOnReleaseLabel);
    setupLabel(file.looped, m_ui->loopedLabel);
    setupLabel(file.choke, m_ui->chokeLabel);
}

quint8 SampleWidget::channel() const
{
    return m_ui->channelSpinBox->value();
}

void SampleWidget::setChannel(quint8 channel)
{
    m_ui->channelSpinBox->setValue(channel);
}

quint8 SampleWidget::note() const
{
    return m_ui->noteSpinBox->value();
}

void SampleWidget::setNote(quint8 note)
{
    m_ui->noteSpinBox->setValue(note);
}

std::optional<int> SampleWidget::choke() const
{
    if (!m_file)
        return {};
    return m_file->choke;
}

void SampleWidget::pressed(quint8 velocity)
{
    Q_UNUSED(velocity)

    if (m_effect)
        QMetaObject::invokeMethod(m_effect.get(), &QSoundEffect::play);

    if (m_file && m_file->choke && *m_file->choke)
        emit chokeTriggered(*m_file->choke);
}

void SampleWidget::released()
{
}

void SampleWidget::forceStop()
{
    if (m_effect)
        QMetaObject::invokeMethod(m_effect.get(), &QSoundEffect::stop);
}

void SampleWidget::setupAudioThread(const QAudioDeviceInfo &device, QThread &thread)
{
    const auto setupEffect = [this,device](){
        m_effect = std::make_unique<QSoundEffect>(device);

        connect(m_effect.get(), &QSoundEffect::playingChanged, this, &SampleWidget::updateStatus);
        connect(m_effect.get(), &QSoundEffect::statusChanged, this, &SampleWidget::updateStatus);

        const auto sampleUrl = this->sampleUrl();
        if (!sampleUrl.isEmpty())
            m_effect->setSource(sampleUrl);

        QMetaObject::invokeMethod(this, &SampleWidget::updateStatus);
    };

    QMetaObject::invokeMethod(QAbstractEventDispatcher::instance(&thread), setupEffect);
    //setupEffect();
}

void SampleWidget::updateStatus()
{
    QPalette pal;
    if (m_effect && m_file && m_file->color)
    {
        const auto bright = m_effect->isPlaying() ? 255 : 155;
        const auto dark = m_effect->isPlaying() ?
#if !defined(Q_OS_WIN)
        80 : 0
#else
        180 : 80
#endif
;

        const auto &color = *m_file->color;
        if (color == "purple")
            pal.setColor(QPalette::Window, QColor{bright, dark, bright});
        else if (color == "red")
            pal.setColor(QPalette::Window, QColor{bright, dark, dark});
        else if (color == "yellow")
            pal.setColor(QPalette::Window, QColor{bright, bright, dark});
        else if (color == "green")
            pal.setColor(QPalette::Window, QColor{dark, bright, dark});
        else if (color == "blue")
            pal.setColor(QPalette::Window, QColor{dark, dark, bright});
        else
            qWarning() << "unknown color:" << color;
    }
    setPalette(pal);

    if (!m_effect)
        m_ui->statusLabel->setText(tr("No player"));
    else
        m_ui->statusLabel->setText(toString(m_effect->status()));
}

QUrl SampleWidget::sampleUrl() const
{
    if (!m_file || !m_file->filename)
        return {};

    return QUrl{QString{"https://brunner.ninja/komposthaufen/dpm/presets/extracted/%0/%1"}.arg(m_presetId, *m_file->filename)};
}
