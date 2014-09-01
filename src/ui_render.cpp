#include "ui_render.hpp"

FractUIRender::FractUIRender() : QWidget(), viewPtr(nullptr) {
    areaRender->setAlignment(Qt::AlignCenter);
    areaRender->setWidget(labelRenderView);
    cbFit->setText("Fit");
    cbFit->setToolTip("Unchecking this essentially doubles memory usage, very large images liable to cause out of memory errors.\nBe aware of your memory usage and capacity.");
    cbFit->setToolTipDuration(0);
    cbFit->setChecked(true);
    QObject::connect(cbFit, SIGNAL(stateChanged(int)), this, SLOT(ResetView()));
    labelRenderView->setMinimumSize(1, 1);
    layoutOverworld->addWidget(areaRender);
    layoutOverworld->addWidget(cbFit);
    this->startTimer(1000/5);
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

void FractUIRender::SetView(const QImage *imgPtr) {
    viewPtr = imgPtr;
    if (viewPtr != nullptr) {
        if (cbFit->checkState() == Qt::Checked && !viewPtr->isNull()) {
            QSize S = areaRender->maximumViewportSize();
            labelRenderView->setPixmap(QPixmap::fromImage(viewPtr->scaled(S, Qt::KeepAspectRatio)));
            labelRenderView->setFixedSize(labelRenderView->pixmap()->size());
        } else {
            QImage &&rimg = viewPtr->copy();
            labelRenderView->setPixmap(QPixmap::fromImage(rimg));
            labelRenderView->setFixedSize(viewPtr->width(), viewPtr->height());
        }
    }
}

void FractUIRender::ResetView() {
    this->SetView(viewPtr);
}

void FractUIRender::RequestUpdate() {
    updateRequested = true;
}

void FractUIRender::timerEvent(QTimerEvent *) {
    if (updateRequested) {
        this->ResetView();
        updateRequested = false;
    }
}
