#include "ui_render.hpp"
#include "ui.hpp"
#include "render.hpp"

FractUIRender::FractUIRender() : QWidget() {
    areaRender->setAlignment(Qt::AlignCenter);
    areaRender->setWidget(labelRenderView);
    cbFit->setText("Fit");
    QObject::connect(cbFit, SIGNAL(stateChanged(int)), this, SLOT(UpdateView()));
    labelRenderView->setMinimumSize(1, 1);
    layoutOverworld->addWidget(areaRender);
    layoutOverworld->addWidget(cbFit);
    this->setLayout(layoutOverworld);
}

QSize FractUIRender::sizeHint() const {
    return QSize(800, 600);
}
void FractUIRender::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    ui::close();
}
void FractUIRender::resizeEvent(QResizeEvent* QRE) {
    if (cbFit->checkState() == Qt::Checked) {
        UpdateView();
    }
    QWidget::resizeEvent(QRE);
}
void FractUIRender::UpdateView() {
    if (cbFit->checkState() == Qt::Checked) {
        QSize S = areaRender->maximumViewportSize();
        labelRenderView->setPixmap(QPixmap::fromImage(render::get_scaled_copy(S.width(), S.height())));
        labelRenderView->setFixedSize(labelRenderView->pixmap()->size());
    } else {
        QImage I = render::get_image_copy();
        labelRenderView->setPixmap(QPixmap::fromImage(I));
        labelRenderView->setFixedSize(I.width(), I.height());
    }
}
