#include "djwidget.h"
#include "ui_djwidget.h"

#include <QStandardPaths>

DjWidget::DjWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{std::make_unique<Ui::DjWidget>()}
{
    m_ui->setupUi(this);

    const auto createSlot = [this](TrackDeck &trackDeck){
        return [this,&trackDeck](){
            const auto index = m_ui->treeViewFiles->currentIndex();
            if (!index.isValid())
                return;
            trackDeck.loadTrack(m_filesModel.filePath(m_filesTableModel.mapToSource(m_sortFilterProxyModel.mapToSource(index))));
        };
    };

    connect(m_ui->pushButtonLoadDeckA, &QAbstractButton::pressed, m_ui->trackDeckA, createSlot(*m_ui->trackDeckA));
    connect(m_ui->pushButtonLoadDeckB, &QAbstractButton::pressed, m_ui->trackDeckB, createSlot(*m_ui->trackDeckB));

    m_directoryModel.setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot);
    const auto rootIndex = m_directoryModel.setRootPath(QDir::homePath());
    m_ui->treeViewDirectories->setModel(&m_directoryModel);
    m_ui->treeViewDirectories->setRootIndex(rootIndex);
    for (auto i = 0, count = m_directoryModel.columnCount(); i < count; i++)
        m_ui->treeViewDirectories->setColumnHidden(i, i!=0);

    connect(m_ui->treeViewDirectories->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &DjWidget::directorySelected);

    m_filesModel.setFilter(QDir::Files);
    m_filesTableModel.setSourceModel(&m_filesModel);
    m_sortFilterProxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_sortFilterProxyModel.setSourceModel(&m_filesTableModel);
    m_ui->treeViewFiles->setModel(&m_sortFilterProxyModel);

    if (const auto locations = QStandardPaths::standardLocations(QStandardPaths::MusicLocation); !locations.isEmpty())
    {
        const auto index = m_directoryModel.index(locations.first());
        m_ui->treeViewDirectories->selectionModel()->select(index, QItemSelectionModel::Clear|QItemSelectionModel::Select|QItemSelectionModel::Current|QItemSelectionModel::Rows);

        const auto rootIndex = m_filesModel.setRootPath(m_directoryModel.filePath(index));
        m_filesTableModel.setRootIndex(rootIndex);
    }

    connect(m_ui->lineEditSearch, &QLineEdit::textChanged, &m_sortFilterProxyModel, &QSortFilterProxyModel::setFilterFixedString);
}

DjWidget::~DjWidget() = default;

void DjWidget::injectDecodingThread(QThread &thread)
{
    m_ui->trackDeckA->injectDecodingThread(thread);
    m_ui->trackDeckB->injectDecodingThread(thread);
}

void DjWidget::writeSamples(frame_t *begin, frame_t *end)
{
    const auto count = std::distance(begin, end);

    for (TrackDeck *trackDeck : {m_ui->trackDeckA, m_ui->trackDeckB})
    {
        frame_t buffer[count];
        std::fill(buffer, buffer+count, frame_t{0.f,0.f});
        trackDeck->writeSamples(buffer, buffer+count);
        std::transform(static_cast<const frame_t *>(begin), static_cast<const frame_t *>(end), buffer, begin, [volume=1.f](const frame_t &frame, const frame_t &frame2){
            frame_t newFrame;
            std::transform(std::cbegin(frame), std::cend(frame), std::begin(frame2), std::begin(newFrame),
                           [&volume](const sample_t &left, const sample_t &right) { return left + (right*volume); });
            return newFrame;
        });
    }
}

void DjWidget::loadSettings(DrumMachineSettings &settings)
{
}

void DjWidget::unsendColors()
{
}

void DjWidget::sendColors()
{
}

void DjWidget::midiReceived(const midi::MidiMessage &message)
{
}

void DjWidget::directorySelected()
{
    const auto selected = m_ui->treeViewDirectories->currentIndex();
    if (selected.isValid())
    {
        const auto rootIndex = m_filesModel.setRootPath(m_directoryModel.filePath(selected));
        m_filesTableModel.setRootIndex(rootIndex);
    }
}
