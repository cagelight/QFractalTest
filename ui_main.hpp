#ifndef UI_MAIN_HPP
#define UI_MAIN_HPP

#include <QWidget>
#include <QtWidgets>

#include "render.hpp"

#include "qwidget_gradient.hpp"
#include "qwidget_2dnavigator.hpp"

class FractUIMain : public QWidget {
    Q_OBJECT
public:
    FractUIMain();
    QSize sizeHint() const;
    void closeEvent(QCloseEvent*);
public slots:
    void BeginRender();
    void StopRender();
    void UpdateProgress();
private:
    //Overworld
    QGridLayout *layoutOverworld = new QGridLayout(this);
    Q2DNavigator *navigatorFractal = new Q2DNavigator(this);
    QColorDialog *colorDialogGradient = new QColorDialog(this);
    QGradientSlider *gradientSliderFractal = new QGradientSlider(this);
    QLineEdit *lineEditGradientPosition = new QLineEdit(this);
    //Renderbar
    QGridLayout *layoutRenderbar = new QGridLayout(this);
    QProgressBar *renderProgressBar = new QProgressBar(this);
    QPushButton *startRender = new QPushButton(this);
    QPushButton *stopRender = new QPushButton(this);
};

#endif // UI_MAIN_HPP
