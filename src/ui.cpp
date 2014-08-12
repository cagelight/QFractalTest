#include "ui.hpp"
#include "debug.hpp"
#include "render.hpp"

#include "ui_main.hpp"
#include "ui_render.hpp"
#include "ui_delegator.hpp"

#include <QApplication>
#include <QtWidgets>

#include <random>

///Static Prototypes///
static void InitializeMainWindow();
static void InitializeRenderWindow();

///Static Variables///
static QApplication* App;
static FractUIDelegator* qtDelegator;
static FractUIMain* windowMain;
static FractUIRender* windowRender;
///::ui///
void ui::initialize(int argc, char* argv[]) {
    App = new QApplication(argc, argv);
    InitializeMainWindow();
    InitializeRenderWindow();
    qtDelegator = new FractUIDelegator();
    QObject::connect(qtDelegator, SIGNAL(UpdateRenderView()), windowRender, SLOT(UpdateView()), Qt::QueuedConnection);
    QObject::connect(qtDelegator, SIGNAL(UpdateProgressBar()), windowMain, SLOT(UpdateProgress()), Qt::QueuedConnection);
}

int ui::execute() {
    return App->exec();
}

void ui::close() {
    if (!App->closingDown())
        App->quit();
}

void ui::terminate() {
    render::terminate();
    delete App;
}

void ui::update_render_view() {
    emit qtDelegator->UpdateRenderView();
}

void ui::update_render_progress() {
    emit qtDelegator->UpdateProgressBar();
}

///Static Definitions///
static void InitializeMainWindow() {
    windowMain = new FractUIMain();
    windowMain->show();
}

static void InitializeRenderWindow() {
    windowRender = new FractUIRender();
    windowRender->show();
}
