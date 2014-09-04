#ifndef UI_PREVIEW_HPP
#define UI_PREVIEW_HPP

#include <QtWidgets>
#include "qwidget_renderpreview.hpp"

class FractUIPreview : public QWidget {
    Q_OBJECT
public:
    FractUIPreview();
    void closeEvent(QCloseEvent*);
signals:
    void closing();
public slots:
    void setMeta(QFractalMeta);
private: //Widgets
    QGridLayout *layout = new QGridLayout(this);
    QRenderPreview *preview = new QRenderPreview(this);
};

#endif // UI_PREVIEW_HPP
