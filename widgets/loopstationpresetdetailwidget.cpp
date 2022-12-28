#include "loopstationpresetdetailwidget.h"
#include "ui_loopstationpresetdetailwidget.h"

#include "loopstationpresets.h"

LoopStationPresetDetailWidget::LoopStationPresetDetailWidget(QWidget *parent) :
    QScrollArea{parent},
    m_ui{std::make_unique<Ui::LoopStationPresetDetailWidget>()}
{
    m_ui->setupUi(this);

    m_ui->listViewTags->setModel(&m_tagsModel);
}

LoopStationPresetDetailWidget::~LoopStationPresetDetailWidget() = default;

void LoopStationPresetDetailWidget::setPreset(const loopstation_presets::Preset &preset)
{
    constexpr const auto applyField = [](QWidget *edit, const auto &field){
        edit->setEnabled(field.has_value());
        edit->setVisible(field.has_value());
    };
    constexpr const auto applyLineEdit = [applyField](QLineEdit *edit, const auto &field){
        applyField(edit, field);
        edit->setText(field ? *field : QString{});
    };
    constexpr const auto applySpinBox = [applyField](QSpinBox *edit, const auto &field){
        applyField(edit, field);
        edit->setValue(field ? *field : -1);
    };
    constexpr const auto applyCheckBox = [applyField](QCheckBox *edit, const auto &field){
        applyField(edit, field);
        edit->setChecked(field ? *field : false);
    };

    applyLineEdit(m_ui->lineEditId, preset.id);
    applyLineEdit(m_ui->lineEditTitle, preset.title);
    applyLineEdit(m_ui->lineEditAudioPreviewUrl, preset.audioPreviewUrl);
    applyLineEdit(m_ui->lineEditAuthor, preset.author);
    applySpinBox(m_ui->spinBoxBpm, preset.bpm);
    // TODO lessons
    applyLineEdit(m_ui->lineEditCoverUrl, preset.coverUrl);
    applySpinBox(m_ui->spinBoxLoopLength, preset.loopLength);
    applyLineEdit(m_ui->lineEditOrderBy, preset.orderBy);
    // TODO pads
    applyCheckBox(m_ui->checkBoxPremium, preset.premium);
    applyCheckBox(m_ui->checkBoxDeleted, preset.DELETED);

    m_tagsModel.setPreset(preset);
}
