#ifndef QWIDGET_RENDERPREVIEW_HPP
#define QWIDGET_RENDERPREVIEW_HPP

#include <QWidget>
#include "qfractalmeta.hpp"

class QRenderPreview : public QWidget {
    Q_OBJECT
public:
    QRenderPreview(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    QSize origSize;
    QFractalMeta *adjustedMeta = nullptr;
public slots:
    void setMeta(QFractalMeta);
private:
    void updateView();
    QPixmap fmap = QPixmap(0, 0);
};

#endif // QWIDGET_RENDERPREVIEW_HPP
