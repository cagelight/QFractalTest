#ifndef QWIDGET_RENDERPREVIEW_HPP
#define QWIDGET_RENDERPREVIEW_HPP

#include <QWidget>

class QRenderPreview : public QWidget {
    Q_OBJECT
public:
    QRenderPreview(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
};

#endif // QWIDGET_RENDERPREVIEW_HPP