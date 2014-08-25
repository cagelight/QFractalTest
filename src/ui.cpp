#include "ui.hpp"
#include "debug.hpp"

#include "ui_main.hpp"
#include "ui_render.hpp"

#include "qobject_fractalrenderer.hpp"

#include <QtWidgets>

#include <random>

FractUIManager::FractUIManager(int argc, char *argv[]) : QApplication(argc, argv) {
    mainWindow = new FractUIMain();
    renderWindow = new FractUIRender();
    mainWindow->show();
    renderWindow->show();
    QObject::connect(mainWindow, SIGNAL(closing()), this, SLOT(quit()));
    QObject::connect(renderWindow, SIGNAL(closing()), this, SLOT(quit()));

    QObject::connect(mainWindow, SIGNAL(startMetaEmit(QFractalMeta)), this, SLOT(beginCoreRender(QFractalMeta)));
    QObject::connect(mainWindow, SIGNAL(stopPressed()), this, SLOT(stopCoreRender()));
}

FractUIManager::~FractUIManager() {
    delete renderWindow;
    delete mainWindow;
}

void FractUIManager::beginCoreRender(QFractalMeta QMF) {
    this->stopCoreRender();
    coreRender = new QFractalRenderer(QMF);
    QObject::connect(coreRender, SIGNAL(finished(QImage)), renderWindow, SLOT(SetView(QImage)));
    QObject::connect(coreRender, SIGNAL(progress(int,int)), mainWindow, SLOT(updateProgress(int,int)));
    coreRender->start();
}

void FractUIManager::stopCoreRender() {
    if (coreRender != nullptr) {
        coreRender->stop();
        delete coreRender;
        coreRender = nullptr;
    }
}
