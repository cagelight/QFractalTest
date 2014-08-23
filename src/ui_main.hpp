#ifndef UI_MAIN_HPP
#define UI_MAIN_HPP

#include <QWidget>
#include <QtWidgets>

#include "qfractalmeta.hpp"
#include "qwidget_gradient.hpp"
#include "qwidget_2dnavigator.hpp"
#include "qwidget_renderpreview.hpp"

class FractUIMain : public QWidget {
    Q_OBJECT
public:
    FractUIMain();
    QSize sizeHint() const;
    void closeEvent(QCloseEvent*);
signals:
    void beginCoreRender(QFractalMeta);
    void stopCoreRender();
    void closing();
private slots:
    void prepareMetaSettings();
private:
    //Overworld
    QGridLayout *layoutOverworld = new QGridLayout(this);
    QColorDialog *colorDialogGradient = new QColorDialog(this);
    QFileDialog *fileDialogSaveImage = new QFileDialog(this, Qt::Dialog);
    QGradientSlider *gradientSliderFractal = new QGradientSlider(this);
    QLineEdit *lineEditGradientPosition = new QLineEdit(this);
    //Navbar
    QGridLayout *layoutNavbar = new QGridLayout(this);
    QRenderPreview *renderPreview = new QRenderPreview(this);
    Q2DNavigator *navigatorFractal = new Q2DNavigator(this, (Q2DNavigatorOptions)(NAVIGATOR2_CENTER | NAVIGATOR2_ZOOM));
    //Renderbar
    QGridLayout *layoutRenderbar = new QGridLayout(this);
    QProgressBar *renderProgressBar = new QProgressBar(this);
    QPushButton *startRender = new QPushButton(this);
    QPushButton *stopRender = new QPushButton(this);
};

#endif // UI_MAIN_HPP
