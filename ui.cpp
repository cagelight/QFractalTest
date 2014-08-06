#include "ui.hpp"
#include "debug.hpp"
#include "render.hpp"

#include "ui_main.hpp"
#include "ui_render.hpp"

#include <QApplication>
#include <QtWidgets>

#include <random>

///Static Prototypes///
static void InitializeMainWindow();
static void InitializeRenderWindow();

///Static Variables///
static QApplication* App;
static FractUIMain* windowMain;
static FractUIRender* windowRender;
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
    if (!App->closingDown())
        App->quit();
}

void ui::terminate() {
    //ui::close();
}

void ui::update_render_view() {
    windowRender->ReRender();
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
