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
    QObject::connect(startRender, SIGNAL(pressed()), this, SLOT(BeginRender()));
    stopRender->setText("Stop");
    QObject::connect(stopRender, SIGNAL(pressed()), this, SLOT(StopRender()));
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
void FractUIMain::BeginRender() {
    /*FS;
    FS.Settings.Width = 2048*6;
    FS.Settings.Height = 2048*6;
    FS.Settings.Iterations = 120;
    FS.Settings.Scale = 8.0f;
    FS.Settings.Offset.X = 0.5f;
    FS.Settings.Offset.Y = 0.0f;

    FS.Gradient = gradientSliderFractal->getGradient();

    render::render(FS);*/
}

void FractUIMain::StopRender() {
    //::render::stop_render();
}

void FractUIMain::UpdateProgress() {
    //RenderProgress RP = ::render::get_progress();
    //float prog = RP.totalProgress();
    //renderProgressBar->setValue((int)(prog * render_progress_bar_max));
    //renderProgressBar->setTextVisible(true);
    //renderProgressBar->setFormat(QString::number(prog*100, 'f', 2) + QString("%"));
}

void FractUIMain::SaveRender(const QString& path) {
    //::render::save_render(path);
}
