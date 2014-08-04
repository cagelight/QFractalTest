#include "ui.hpp"
#include "debug.hpp"
#include "render.hpp"
#include "qwidget_gradient.hpp"

#include <QApplication>
#include <QtWidgets>

#include <random>

///Static Prototypes///
static void InitializeMainWindow();
static void InitializeRenderWindow();

///Static Variables///
static QApplication* App;
//MainWindow
static ui::classes::MainWindow* windowMain;
static QGridLayout* layoutMain;
static QPushButton* buttonRender;
static QGradientSlider* gradientFractal;
static QLineEdit* gradientPosLine;
static QColorDialog* dialogGradientColor;
//RenderWindow
static bool fitView = false;
static ui::classes::RenderWindow* windowRender;
static QGridLayout* layoutRender;
static QHBoxLayout* layoutRenderViewOptions;
static QCheckBox* cbRenderView;
static QLabel* labelRenderView;
static QScrollArea* areaRender;
///::ui///
void ui::initialize(int argc, char* argv[]) {
    App = new QApplication(argc, argv);
    InitializeMainWindow();
    InitializeRenderWindow();
}

int ui::execute() {
    return App->exec();
}

void ui::close() {
    App->quit();
}

void ui::terminate() {
    delete App;
}

void ui::update_render_view() {
    if (fitView) {
        QSize S = areaRender->viewport()->rect().size();
        labelRenderView->setPixmap(QPixmap::fromImage(render::get_scaled_copy(S.width(), S.height())));
        labelRenderView->setFixedSize(labelRenderView->pixmap()->size());
    } else {
        QImage I = render::get_image_copy();
        labelRenderView->setPixmap(QPixmap::fromImage(I));
        labelRenderView->setFixedSize(I.width(), I.height());
    }
}

///Window Class Definitions///
//MainWindow Definitions
ui::classes::MainWindow::MainWindow() : QWidget() {}
QSize ui::classes::MainWindow::sizeHint() const {
    return QSize(400, 300);
}
void ui::classes::MainWindow::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    ui::close();
}
void ui::classes::MainWindow::Render() {
    fractal::Settings FS;
    FS.Width = 4096;
    FS.Height = 4096;
    FS.Iterations = 80;
    FS.Scale = 0.125f;
    FS.Offset.x = 1.5f;

    FS.Gradient = gradientFractal->getGradient();

    render::render(FS);
}
//RenderWindow Definitions
ui::classes::RenderWindow::RenderWindow() : QWidget() {}
QSize ui::classes::RenderWindow::sizeHint() const {
    return QSize(800, 600);
}
void ui::classes::RenderWindow::closeEvent(QCloseEvent* QCE) {
    QWidget::closeEvent(QCE);
    ui::close();
}
void ui::classes::RenderWindow::resizeEvent(QResizeEvent* QRE) {
    if (fitView) {
        ui::update_render_view();
    }
    QWidget::resizeEvent(QRE);
}
void ui::classes::RenderWindow::SetRenderViewMode(int state) {
    fitView = state;
    ui::update_render_view();
}

///Static Definitions///
static void InitializeMainWindow() {
    windowMain = new ui::classes::MainWindow();
    layoutMain = new QGridLayout(windowMain);
    buttonRender = new QPushButton("Render", windowMain);
    QObject::connect(buttonRender, SIGNAL(pressed()), windowMain, SLOT(Render()));
    layoutMain->addWidget(buttonRender);
    gradientFractal = new QGradientSlider(windowMain);
    layoutMain->addWidget(gradientFractal);
    gradientPosLine = new QLineEdit(windowMain);
    gradientPosLine->setValidator(new QDoubleValidator(windowMain));
    QObject::connect(gradientFractal, SIGNAL(SelectedPositionChanged(QString)), gradientPosLine, SLOT(setText(QString)));
    QObject::connect(gradientPosLine, SIGNAL(textEdited(QString)), gradientFractal, SLOT(SetSelectedPosition(QString)));
    layoutMain->addWidget(gradientPosLine);
    dialogGradientColor = new QColorDialog();
    QObject::connect(gradientFractal, SIGNAL(HandleDoubleClicked()), dialogGradientColor, SLOT(open()));
    QObject::connect(dialogGradientColor, SIGNAL(colorSelected(QColor)), gradientFractal, SLOT(SetSelectedColor(QColor)));
    windowMain->setLayout(layoutMain);
    windowMain->show();
}

static void InitializeRenderWindow() {
    windowRender = new ui::classes::RenderWindow();
    layoutRender = new QGridLayout(windowRender);
    layoutRenderViewOptions = new QHBoxLayout(windowRender);
    layoutRenderViewOptions->setSizeConstraint(QLayout::SetFixedSize);
    areaRender = new QScrollArea(windowRender);
    areaRender->setAlignment(Qt::AlignCenter);
    labelRenderView = new QLabel(windowRender);
    labelRenderView->setMinimumSize(1, 1);
    areaRender->setWidget(labelRenderView);
    layoutRender->addWidget(areaRender);
    cbRenderView = new QCheckBox("Fit", windowRender);
    QObject::connect(cbRenderView, SIGNAL(stateChanged(int)), windowRender, SLOT(SetRenderViewMode(int)));
    layoutRenderViewOptions->addWidget(cbRenderView);
    layoutRender->addLayout(layoutRenderViewOptions, 1, 0);
    windowRender->setLayout(layoutRender);
    windowRender->show();
}
