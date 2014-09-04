#ifndef UI_MAIN_HPP
#define UI_MAIN_HPP

#include <QWidget>
#include <QtWidgets>

#include "qfractalmeta.hpp"
#include "qwidget_gradient.hpp"
#include "qwidget_navigator2d.hpp"

class FractUIMain : public QWidget {
    Q_OBJECT
public:
    FractUIMain();
    QSize sizeHint() const;
    void closeEvent(QCloseEvent*);
signals:
    void coreRenderRequested(QFractalMeta);
    void previewUpdateRequested(QFractalMeta);
    void stopPressed();
    void closing();
public slots:
    void updateProgressBar(int, int);
    void requestUpdateProgressBar(int, int);
    void requestPreviewUpdate();
private slots:
    void requestCoreRender();
    void metaOffsetDir(QNavigator2D::Direction);
private://FractalMeta
    QPointF metaOffset = QPointF(0.0f, 0.0f);
    float metaScale = 1.0f;
    QFractalMeta formalizeMeta();
private:
    //Update Notification
    bool updateRequested = false;
    int recCur, recMax;
    void timerEvent(QTimerEvent*);
    //Overworld
    QGridLayout *layoutOverworld = new QGridLayout(this);
    QColorDialog *colorDialogGradient = new QColorDialog(this);
    QFileDialog *fileDialogSaveImage = new QFileDialog(this, Qt::Dialog);
    QGradientSlider *gradientSliderFractal = new QGradientSlider(this);
    QLineEdit *lineEditGradientPosition = new QLineEdit(this);
    //Navbar
    QGridLayout *layoutNavbar = new QGridLayout(this);
    QNavigator2D *navigatorFractal = new QNavigator2D(this, (QNavigator2D::Options)(QNavigator2D::NAVIGATOR2_CENTER | QNavigator2D::NAVIGATOR2_ZOOM));
    //Renderbar
    QGridLayout *layoutRenderbar = new QGridLayout(this);
    QProgressBar *renderProgressBar = new QProgressBar(this);
    QPushButton *startRender = new QPushButton(this);
    QPushButton *stopRender = new QPushButton(this);
};

#endif // UI_MAIN_HPP
