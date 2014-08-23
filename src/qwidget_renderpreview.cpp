#include "qwidget_renderpreview.hpp"

QRenderPreview::QRenderPreview(QWidget *parent) : QWidget(parent) {
    this->setMinimumSize(1, 1);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void QRenderPreview::paintEvent(QPaintEvent * QPE) {

}
