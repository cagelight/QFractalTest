#include "ui_main.hpp"
#include "common.hpp"
#include "multigradient.hpp"

static const int render_progress_bar_max = 10000;

FractUIMain::FractUIMain() : QWidget() {
    //Overworld
    QObject::connect(gradientSliderFractal, SIGNAL(HandleDoubleClicked()), colorDialogGradient, SLOT(open()));
    QObject::connect(colorDialogGradient, SIGNAL(colorSelected(QColor)), gradientSliderFractal, SLOT(SetSelectedColor(QColor)));
    QObject::connect(gradientSliderFractal, SIGNAL(SelectedPositionChanged(QString)), lineEditGradientPosition, SLOT(setText(QString)));
    QObject::connect(lineEditGradientPosition, SIGNAL(textEdited(QString)), gradientSliderFractal, SLOT(SetSelectedPosition(QString)));
    renderProgressBar->setMinimum(0);
    renderProgressBar->setMaximum(render_progress_bar_max);
    startRender->setText("Render");
    QObject::connect(startRender, SIGNAL(pressed()), this, SLOT(formalizeMeta()));
    stopRender->setText("Stop");
    QObject::connect(stopRender, SIGNAL(pressed()), this, SIGNAL(stopPressed()));
    lineEditGradientPosition->setValidator(new QDoubleValidator(this));
    //Navbar
    layoutNavbar->addWidget(renderPreview, 0, 0);
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
}
QSize FractUIMain::sizeHint() const {
    return QSize(400, 300);
}
void FractUIMain::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    emit closing();
}

void FractUIMain::updateProgress(int cur, int max) {
    this->renderProgressBar->setMaximum(max);
    this->renderProgressBar->setValue(cur);
    this->renderProgressBar->setFormat(QString::number((cur/(float)max)*100.0f, 'f', 2)+QString("%"));
}

void FractUIMain::formalizeMeta() {
    QSize size(4096, 4096);
    float scale = 3.0f;
    QPointF offset(0.0f, 0.0f);
    unsigned int iterations = 80;
    fract_pass fp(STOP_MANDELBROT, {FUNC_MANDELBROT});
    emit startMetaEmit(QFractalMeta(size, scale, offset, iterations, gradientSliderFractal->getGradient(), fp));
}
