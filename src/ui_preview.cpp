#include "ui_preview.hpp"

FractUIPreview::FractUIPreview() {
    this->setWindowTitle("QFT Preview");
    this->setLayout(layout);
    layout->addWidget(preview);

    this->setMinimumSize(68, 68);
    layout->setMargin(4);
    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, QColor(0, 0, 0));
    this->setPalette(pal);
}

void FractUIPreview::closeEvent(QCloseEvent *QCE) {
    emit closing();
    QWidget::closeEvent(QCE);
}

void FractUIPreview::setMeta(QFractalMeta QMF) {
    this->preview->setMeta(QMF);
}
