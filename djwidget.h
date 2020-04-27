#pragma once

#include <QWidget>
#include <QFileSystemModel>

#include <memory>

#include "audioformat.h"
#include "flatfilesystemmodel.h"

namespace Ui { class DjWidget; }

class DjWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DjWidget(QWidget *parent = nullptr);
    ~DjWidget() override;

    void injectDecodingThread(QThread &thread);

    void writeSamples(frame_t *begin, frame_t *end);

private slots:
    void directorySelected();

private:
    const std::unique_ptr<Ui::DjWidget> m_ui;

    QFileSystemModel m_directoryModel;
    FlatFileSystemModel m_filesModel;
};
