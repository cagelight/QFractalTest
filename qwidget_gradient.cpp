#include "qwidget_gradient.hpp"

#include <QtGui>

static const int handleDiameter = 4;

QGradientSlider::QGradientSlider(QWidget *parent) :
    QWidget(parent),
    gradient{
        GradientNode(0.0f, Color(255, 255, 255, 0)),
        GradientNode(1.0f, Color(255, 255, 0, 0)),
        GradientNode(2.0f, Color(255, 255, 255, 255))},
    gradmm(gradient.GetRange())
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    selectedNode = nullptr;
}

void QGradientSlider::paintEvent(QPaintEvent* QPE) {
    QRect gradientRect = QPE->rect().adjusted(0, handleDiameter, 0, -handleDiameter);
    this->updateGradientImage(gradientRect);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QBrush brushBlack(QColor(0, 0, 0), Qt::SolidPattern);
    painter.drawImage(gradientRect, gradmap);
    for (const GradientNode* v : gradient.GetVector()) {
        int hpos = (v->first - gradmm.min) / gradmm.max * (this->width() - 2*handleDiameter) + handleDiameter;
        if (v == selectedNode) {
            Color C = selectedNode->second;
            painter.setBrush(QBrush(QColor(255-C.R, 255-C.G, 255-C.B), Qt::SolidPattern));
            painter.drawRect(hpos-handleDiameter-1, -1, (handleDiameter + 1) * 2, (handleDiameter + 1) * 2);
            painter.drawRect(hpos-1, handleDiameter+2, 2, QPE->rect().height() - (2 * handleDiameter) - 2);
            painter.setBrush(brushBlack);
            painter.drawRect(hpos-handleDiameter, 0, handleDiameter * 2, handleDiameter * 2);
        } else {
            painter.setBrush(brushBlack);
            painter.drawRect(hpos-handleDiameter, 0, handleDiameter * 2, handleDiameter * 2);
        }
    }
}

QSize QGradientSlider::sizeHint() const {
    return QSize(128, 32);
}

void QGradientSlider::mousePressEvent(QMouseEvent* QME) {
    QPoint locPos = QPoint((int)QME->localPos().x(), (int)QME->localPos().y());
    //QPointF gradPos = QPointF((locPos.x() / this->width() * (gradmm.max - gradmm.min)) + gradmm.min, locPos.y());
    if (locPos.y() > 2 * (handleDiameter + 1)) {
        selectedNode = nullptr;
    } else {
        bool fflag = false;
        for (const GradientNode* h : gradient.GetVector()) {
            int handleHPos = (h->first - gradmm.min) / gradmm.max * (this->width() - 2*handleDiameter) + handleDiameter;
            if (locPos.x() < handleHPos + handleDiameter && locPos.x() > handleHPos - handleDiameter) {
                if (QME->button() == Qt::LeftButton) {
                    selectedNode = h;
                } else if (QME->button() == Qt::RightButton) {
                    if (selectedNode == h) selectedNode = nullptr;
                    gradient.Remove(h);
                }
                fflag = true;
                break;
            }
        }
        if (!fflag && QME->button() == Qt::LeftButton) {
            float newPos = std::min(std::max(((float)QME->localPos().x() / this->width()) * (gradmm.max - gradmm.min) + gradmm.min, gradmm.min), gradmm.max);
            Color newColor = gradient.Lerp(newPos);
            const GradientNode* N = gradient.Add(newPos, newColor);
            selectedNode = N;
        }
    }
    this->repaint();
    selectedPressed = (selectedNode != nullptr);
    QWidget::mousePressEvent(QME);
}

void QGradientSlider::mouseReleaseEvent(QMouseEvent* QME) {
    selectedPressed = false;
}

void QGradientSlider::mouseMoveEvent(QMouseEvent* QME) {
    if (selectedPressed) {
        float newPos = std::min(std::max(((float)QME->localPos().x() / this->width()) * (gradmm.max - gradmm.min) + gradmm.min, gradmm.min), gradmm.max);
        gradient.Set(selectedNode, newPos);
        this->repaint();
    }
}

void QGradientSlider::setGradient(const MultiGradient& MG) {
    this->gradient = MG;
    this->gradmm = MG.GetRange();
}

MultiGradient QGradientSlider::getGradient() const {
    MultiGradient MG = gradient;
    Range MGR = MG.GetRange();
    if (gradmm.min != MGR.min)
        MG.Add(gradmm.min, MG.Lerp(gradmm.min));
    if (gradmm.max != MGR.max)
        MG.Add(gradmm.max, MG.Lerp(gradmm.max));
    return MG;
}

void QGradientSlider::updateGradientImage(const QRect& rect) {
    this->gradmap = QImage(rect.size(), QImage::Format_ARGB32);
    Color* horizmap = this->gradient.Bake(gradmm.min, gradmm.max, gradmap.width(), handleDiameter);
    for (int row = 0; row < gradmap.height(); row++) {
        uint* rowPtr = (uint*)gradmap.scanLine(row);
        for (int col = 0; col < gradmap.width(); col++) {
            rowPtr[col] = horizmap[col];
        }
    }
    delete[] horizmap;
}
