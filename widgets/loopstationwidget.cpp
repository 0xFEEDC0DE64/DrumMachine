#include "loopstationwidget.h"
#include "ui_loopstationwidget.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>

#include "audioformat.h"
#include "midicontainers.h"
#include "jsonconverters.h"
#include "loopstationjsonconverters.h"
#include "drummachinesettings.h"

LoopStationWidget::LoopStationWidget(QWidget *parent) :
    QSplitter{parent},
    m_ui{std::make_unique<Ui::LoopStationWidget>()}
{
    m_ui->setupUi(this);

    connect(m_ui->pushButtonUp, &QAbstractButton::pressed, this, &LoopStationWidget::selectPrevPreset);
    connect(m_ui->pushButtonDown, &QAbstractButton::pressed, this, &LoopStationWidget::selectNextPreset);
    connect(m_ui->pushButtonRefresh, &QAbstractButton::pressed, this, &LoopStationWidget::loadPresets);
}

LoopStationWidget::~LoopStationWidget() = default;

void LoopStationWidget::writeSamples(frame_t *begin, frame_t *end)
{

}

void LoopStationWidget::injectNetworkAccessManager(QNetworkAccessManager &networkAccessManager)
{
    m_networkAccessManager = &networkAccessManager;
    loadPresets();
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

void LoopStationWidget::loadPresets()
{
    if (!m_networkAccessManager)
    {
        qWarning() << "no network access manager available";
        return;
    }

    m_ui->pushButtonRefresh->setEnabled(false);

    QNetworkRequest request{QUrl{"https://brunner.ninja/komposthaufen/groovepad/presets_config_v2.json"}};
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);

    m_reply = std::unique_ptr<QNetworkReply>(m_networkAccessManager->get(request));
    connect(m_reply.get(), &QNetworkReply::finished, this, &LoopStationWidget::requestFinished);
}

void LoopStationWidget::requestFinished()
{
    if (!m_reply)
    {
        qWarning() << "no valid reply";
        return;
    }

    auto reply = std::move(m_reply);
    if (reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(this, tr("Could not load presets!"), tr("Could not load presets!") + "\n\n" + reply->errorString());
        return;
    }

    try
    {
        auto result = json_converters::loopstation::parsePresetsConfig(json_converters::loadJson(reply->readAll()));

        if (!result.presets)
            throw std::runtime_error("presets missing in response");

        // TODO
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, tr("error"), tr("error") + "\n\n" + QString::fromStdString(e.what()));
    }
}

void LoopStationWidget::selectFirstPreset()
{

}

void LoopStationWidget::selectPrevPreset()
{

}

void LoopStationWidget::selectNextPreset()
{

}
