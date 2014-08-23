#include "ui.hpp"
#include "debug.hpp"

#include "ui_main.hpp"
#include "ui_render.hpp"

#include <QtWidgets>

#include <random>

FractUIManager::FractUIManager(int argc, char *argv[]) : QApplication(argc, argv) {
    mainWindow = new FractUIMain();
    renderWindow = new FractUIRender();
    mainWindow->show();
    renderWindow->show();
    QObject::connect(mainWindow, SIGNAL(closing()), this, SLOT(quit()));
    QObject::connect(renderWindow, SIGNAL(closing()), this, SLOT(quit()));
}

FractUIManager::~FractUIManager() {
    delete renderWindow;
    delete mainWindow;
}
