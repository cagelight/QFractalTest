#include "ui_render.hpp"

FractUIRender::FractUIRender() : QWidget(), localCopy(0, 0) {
    areaRender->setAlignment(Qt::AlignCenter);
    areaRender->setWidget(labelRenderView);
    cbFit->setText("Fit");
    QObject::connect(cbFit, SIGNAL(stateChanged(int)), this, SLOT(ResetView()));
    labelRenderView->setMinimumSize(1, 1);
    layoutOverworld->addWidget(areaRender);
    layoutOverworld->addWidget(cbFit);
}

QSize FractUIRender::sizeHint() const {
    return QSize(800, 600);
}
void FractUIRender::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    emit closing();
}
void FractUIRender::resizeEvent(QResizeEvent* QRE) {
    if (cbFit->checkState() == Qt::Checked) {
        this->ResetView();
    }
    QWidget::resizeEvent(QRE);
}

void FractUIRender::SetView(const QImage& img) {
    localCopy = img;
    if (cbFit->checkState() == Qt::Checked && !img.isNull()) {
        QSize S = areaRender->maximumViewportSize();
        labelRenderView->setPixmap(QPixmap::fromImage(img.scaled(S, Qt::KeepAspectRatio)));
        labelRenderView->setFixedSize(labelRenderView->pixmap()->size());
    } else {
        labelRenderView->setPixmap(QPixmap::fromImage(img));
        labelRenderView->setFixedSize(img.width(), img.height());
    }
}

void FractUIRender::ResetView() {
    this->SetView(localCopy);
}
