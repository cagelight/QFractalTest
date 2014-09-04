#include "qwidget_renderpreview.hpp"
#include "qobject_fractalrenderer.hpp"

#include <QtGui>

QRenderPreview::QRenderPreview(QWidget *parent) : QWidget(parent) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void QRenderPreview::paintEvent(QPaintEvent * QPE) {
    QPainter paint(this);
    if (fmap.size() != this->size()) {
        this->updateView();
    }
    QPoint center((this->width() - fmap.width())/2, (this->height() - fmap.height())/2 );
    paint.drawPixmap(center.x(), center.y(), fmap.width(), fmap.height(), fmap);
}

void QRenderPreview::resizeEvent(QResizeEvent *QRE) {
    this->updateView();
    this->repaint();
}

void QRenderPreview::setMeta(QFractalMeta QFM) {
    if (this->adjustedMeta != nullptr) delete adjustedMeta;
    this->origSize = QFM.size;
    this->adjustedMeta = new QFractalMeta(QFM);
    //adjustedMeta->iterations /= 2;
    if (this->isVisible()) {
        this->updateView();
        this->repaint();
    }
}


void QRenderPreview::updateView() {
    if (this->adjustedMeta != nullptr) {
        this->adjustedMeta->size = origSize.scaled(this->size(), Qt::KeepAspectRatio);
        fmap = QPixmap::fromImage(QFractalRenderer::renderMeta(*adjustedMeta, 0));
    }
}
