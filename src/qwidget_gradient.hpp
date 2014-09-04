#ifndef QWIDGET_GRADIENT_HPP
#define QWIDGET_GRADIENT_HPP

#include "common.hpp"
#include "multigradient.hpp"

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
    void mouseDoubleClickEvent(QMouseEvent*);
    //New Methods
    void setGradient(const MultiGradient&);
    MultiGradient getGradient() const;
private:
    bool selectedPressed = false;
    QImage gradmap;
    void updateGradientImage(const QRect&);
    MultiGradient gradient;
    Range gradmm;
    const GradientNode* selectedNode;
    void SetSelectedNode(const GradientNode*);
    void UnsetSelectedNode();
    void UpdatePositionSignal();
public slots:
    bool SetSelectedColor(QColor newColorQ);
    bool SetSelectedPosition(QString newPosStr);
signals:
    void SelectedPositionChanged(QString newPosStr);
    void HandleDoubleClicked();
    void changed();
};

#endif // QWIDGET_GRADIENT_HPP
