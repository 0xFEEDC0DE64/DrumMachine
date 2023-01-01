#include "drumpadpresetdetailwidget.h"
#include "ui_drumpadpresetdetailwidget.h"

#include "drumpadpresets.h"

DrumPadPresetDetailWidget::DrumPadPresetDetailWidget(QWidget *parent) :
    QScrollArea{parent},
    m_ui{std::make_unique<Ui::DrumPadPresetDetailWidget>()},
    m_tagsModel{this}
{
    m_ui->setupUi(this);

    m_ui->listViewTags->setModel(&m_tagsModel);
}

DrumPadPresetDetailWidget::~DrumPadPresetDetailWidget() = default;

void DrumPadPresetDetailWidget::setPreset(const drumpad_presets::Preset &preset)
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
    constexpr const auto applyDateTimeEdit = [applyField](QDateTimeEdit *edit, const auto &field){
        applyField(edit, field);
        edit->setDateTime(field ? *field : QDateTime{});
    };

    applyLineEdit(m_ui->lineEditId, preset.id);
    applyLineEdit(m_ui->lineEditName, preset.name);
    applyLineEdit(m_ui->lineEditAuthor, preset.author);
    applyLineEdit(m_ui->lineEditOrderBy, preset.orderBy);
    applyLineEdit(m_ui->lineEditVersion, preset.version);
    applySpinBox(m_ui->spinBoxTempo, preset.tempo);
    applyLineEdit(m_ui->lineEditIcon, preset.icon);
    applySpinBox(m_ui->spinBoxPrice, preset.price);
    applySpinBox(m_ui->spinBoxPriceForSession, preset.priceForSession);
    applyCheckBox(m_ui->checkBoxHasInfo, preset.hasInfo);
    applyField(m_ui->listViewTags, preset.tags);
    applyCheckBox(m_ui->checkBoxDeleted, preset.DELETED);
    applySpinBox(m_ui->spinBoxDifficulty, preset.difficulty);
    applySpinBox(m_ui->spinBoxSample, preset.sample);
    applyLineEdit(m_ui->lineEditAudioPreview1Name, preset.audioPreview1Name);
    applyLineEdit(m_ui->lineEditAudioPreview1URL, preset.audioPreview1URL);
    applyLineEdit(m_ui->lineEditAudioPreview2Name, preset.audioPreview2Name);
    applyLineEdit(m_ui->lineEditAudioPreview2URL, preset.audioPreview2URL);
    applyLineEdit(m_ui->lineEditImagePreview1, preset.imagePreview1);
    applyLineEdit(m_ui->lineEditVideoPreview, preset.videoPreview);
    applyLineEdit(m_ui->lineEditVideoTutorial, preset.videoTutorial);
    // TODO files
    // TODO beatschool
    // TODO easyPlay
    applyDateTimeEdit(m_ui->dateTimeEditTimestamp, preset.timestamp);

    m_tagsModel.setPreset(preset);
}
