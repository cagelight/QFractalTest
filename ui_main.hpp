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
    void UpdateProgress();
private:
    QGridLayout *layoutOverworld = new QGridLayout(this);
    Q2DNavigator *navigatorFractal = new Q2DNavigator(this);
    QColorDialog *colorDialogGradient = new QColorDialog(this);
    QGradientSlider *gradientSliderFractal = new QGradientSlider(this);
    QLineEdit *lineEditGradientPosition = new QLineEdit(this);
    QProgressBar* renderProgress = new QProgressBar(this);
    QPushButton *pushButtonRender = new QPushButton(this);
};

#endif // UI_MAIN_HPP
