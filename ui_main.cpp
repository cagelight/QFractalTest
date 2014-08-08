#include "ui.hpp"
#include "ui_main.hpp"
#include "fractal.hpp"
#include "common.hpp"
#include "render.hpp"

static const int render_progress_bar_max = 10000;

FractUIMain::FractUIMain() : QWidget() {
    QObject::connect(gradientSliderFractal, SIGNAL(HandleDoubleClicked()), colorDialogGradient, SLOT(open()));
    QObject::connect(colorDialogGradient, SIGNAL(colorSelected(QColor)), gradientSliderFractal, SLOT(SetSelectedColor(QColor)));
    QObject::connect(gradientSliderFractal, SIGNAL(SelectedPositionChanged(QString)), lineEditGradientPosition, SLOT(setText(QString)));
    QObject::connect(lineEditGradientPosition, SIGNAL(textEdited(QString)), gradientSliderFractal, SLOT(SetSelectedPosition(QString)));
    renderProgress->setMinimum(0);
    renderProgress->setMaximum(render_progress_bar_max);
    pushButtonRender->setText("Render");
    QObject::connect(pushButtonRender, SIGNAL(pressed()), this, SLOT(BeginRender()));
    lineEditGradientPosition->setValidator(new QDoubleValidator(this));
    layoutOverworld->addWidget(navigatorFractal);
    layoutOverworld->addWidget(gradientSliderFractal);
    layoutOverworld->addWidget(lineEditGradientPosition);
    layoutOverworld->addWidget(pushButtonRender);
    layoutOverworld->addWidget(renderProgress);
    this->setLayout(layoutOverworld);
}
QSize FractUIMain::sizeHint() const {
    return QSize(400, 300);
}
void FractUIMain::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    ui::close();
}
void FractUIMain::BeginRender() {
    fractal::Settings FS;
    FS.Width = 2048;
    FS.Height = 2048;
    FS.Iterations = 60;
    //FS.Scale = 0.125f;
    //FS.Offset.x = 1.5f;

    FS.Gradient = gradientSliderFractal->getGradient();

    render::render(FS);
}

void FractUIMain::UpdateProgress() {
    RenderProgress RP = ::render::get_progress();
    float prog = RP.totalProgress();
    renderProgress->setValue((int)(prog * render_progress_bar_max));
    renderProgress->setTextVisible(true);
    renderProgress->setFormat(QString::number(prog*100, 'f', 2) + QString("%"));
}
