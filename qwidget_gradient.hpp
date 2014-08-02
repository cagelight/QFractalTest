#ifndef QWIDGET_GRADIENT_HPP
#define QWIDGET_GRADIENT_HPP

#include "common.hpp"

#include <QWidget>

class QGradientSliderHandle {
public:
    const GradientNode* Node;
    QGradientSliderHandle(const GradientNode* Node) : Node(Node) {}
};

class QGradientSlider : public QWidget {
    Q_OBJECT
public:
    //QWidget Implementations
    explicit QGradientSlider(QWidget *parent = 0);
    void paintEvent(QPaintEvent*);
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    //New Methods
    void setGradient(const MultiGradient&);
    MultiGradient getGradient() const;
private:
    bool selectedPressed = false;
    QImage gradmap;
    void updateGradientImage(const QRect&);
    MultiGradient gradient;
    Range gradmm;
    std::vector<QGradientSliderHandle> handles;
    QGradientSliderHandle* selectedHandle;
};

#endif // QWIDGET_GRADIENT_HPP
