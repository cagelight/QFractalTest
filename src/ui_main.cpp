#include "ui_main.hpp"
#include "common.hpp"
#include "multigradient.hpp"

#include "debug.hpp"

FractUIMain::FractUIMain() : QWidget() {
    this->setWindowTitle("QFT Settings");
    //Overworld
    QObject::connect(gradientSliderFractal, SIGNAL(HandleDoubleClicked()), colorDialogGradient, SLOT(open()));
    QObject::connect(colorDialogGradient, SIGNAL(colorSelected(QColor)), gradientSliderFractal, SLOT(SetSelectedColor(QColor)));
    QObject::connect(gradientSliderFractal, SIGNAL(SelectedPositionChanged(QString)), lineEditGradientPosition, SLOT(setText(QString)));
    QObject::connect(lineEditGradientPosition, SIGNAL(textEdited(QString)), gradientSliderFractal, SLOT(SetSelectedPosition(QString)));
    renderProgressBar->setMinimum(0);
    renderProgressBar->setMaximum(1);
    startRender->setText("Render");
    QObject::connect(startRender, SIGNAL(pressed()), this, SLOT(requestCoreRender()));
    stopRender->setText("Stop");
    QObject::connect(stopRender, SIGNAL(pressed()), this, SIGNAL(stopPressed()));
    lineEditGradientPosition->setValidator(new QDoubleValidator(this));
    QObject::connect(navigatorFractal, SIGNAL(directionReleased(QNavigator2D::Direction)), this, SLOT(metaOffsetDir(QNavigator2D::Direction)));
    //Navbar
    QObject::connect(gradientSliderFractal, SIGNAL(changed()), this, SLOT(requestPreviewUpdate()));
    layoutNavbar->addWidget(navigatorFractal, 0, 1);
    //Renderbar
    layoutRenderbar->addWidget(startRender, 0, 0);
    layoutRenderbar->addWidget(renderProgressBar, 0, 1);
    layoutRenderbar->addWidget(stopRender, 0, 2);
    //Overworld Layouting
    layoutOverworld->addLayout(layoutNavbar, 0, 0);
    layoutOverworld->addWidget(gradientSliderFractal);
    layoutOverworld->addWidget(lineEditGradientPosition);
    layoutOverworld->addLayout(layoutRenderbar, 99, 0);
    this->setLayout(layoutOverworld);
    this->startTimer(1000/60);
}
QSize FractUIMain::sizeHint() const {
    return QSize(400, 300);
}
void FractUIMain::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    emit closing();
}

void FractUIMain::updateProgressBar(int cur, int max) {
    this->renderProgressBar->setMaximum(max);
    this->renderProgressBar->setValue(cur);
    this->renderProgressBar->setFormat(QString::number((cur/(float)max)*100.0f, 'f', 2)+QString("%"));
}

QFractalMeta FractUIMain::formalizeMeta() {
    QSize size(512, 512);
    unsigned int iterations = 60;
    fract_pass fp(STOP_MANDELBROT, {FUNC_MANDELBROT});
    return QFractalMeta(size, metaScale, metaOffset, iterations, gradientSliderFractal->getGradient(), fp);
}

void FractUIMain::requestCoreRender() {
    QFractalMeta QFM = formalizeMeta();
    emit coreRenderRequested(QFM);
}

void FractUIMain::requestPreviewUpdate() {
    QFractalMeta QFM = formalizeMeta();
    emit previewUpdateRequested(QFM);
}

void FractUIMain::metaOffsetDir(QNavigator2D::Direction dir) {
    float varmount = 0.125f * metaScale;
    switch (dir) {
    case QNavigator2D::NAV_UP:
        metaOffset.ry() += varmount;
        break;
    case QNavigator2D::NAV_DOWN:
        metaOffset.ry() -= varmount;
        break;
    case QNavigator2D::NAV_LEFT:
        metaOffset.rx() += varmount;
        break;
    case QNavigator2D::NAV_RIGHT:
        metaOffset.rx() -= varmount;
        break;
    case QNavigator2D::NAV_CENTER:
        metaOffset = QPointF(0.0f, 0.0f);
        metaScale = 1.0f;
        break;
    case QNavigator2D::NAV_INC:
        metaScale /= 1.25f;
        break;
    case QNavigator2D::NAV_DEC:
        metaScale *= 1.25f;
        break;
    }
    emit previewUpdateRequested(formalizeMeta());
}

void FractUIMain::requestUpdateProgressBar(int cur, int max) {
    recCur = cur; recMax = max;
    updateRequested = true;
}

void FractUIMain::timerEvent(QTimerEvent *QTE) {
    if (updateRequested) {
        this->updateProgressBar(recCur, recMax);
        updateRequested = false;
    }
}
