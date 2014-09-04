#include "ui.hpp"
#include "debug.hpp"

#include "ui_main.hpp"
#include "ui_render.hpp"
#include "ui_preview.hpp"
#include "qobject_fractalrenderer.hpp"

#include <QtWidgets>

#include <random>

FractUIManager::FractUIManager(int argc, char *argv[]) : QApplication(argc, argv) {

    mainWindow = new FractUIMain();
    renderWindow = new FractUIRender();
    previewWindow = new FractUIPreview();
    mainWindow->show();
    renderWindow->show();
    previewWindow->show();
    QObject::connect(mainWindow, SIGNAL(closing()), this, SLOT(quit()));
    QObject::connect(renderWindow, SIGNAL(closing()), this, SLOT(quit()));
    QObject::connect(previewWindow, SIGNAL(closing()), this, SLOT(quit()));
    QObject::connect(mainWindow, SIGNAL(coreRenderRequested(QFractalMeta)), this, SLOT(beginCoreRender(QFractalMeta)));
    QObject::connect(mainWindow, SIGNAL(stopPressed()), this, SLOT(stopCoreRender()));
    QObject::connect(mainWindow, SIGNAL(previewUpdateRequested(QFractalMeta)), previewWindow, SLOT(setMeta(QFractalMeta)));

    mainWindow->requestPreviewUpdate();
}

FractUIManager::~FractUIManager() {
    delete renderWindow;
    delete mainWindow;
}

void FractUIManager::beginCoreRender(QFractalMeta QMF) {
    this->stopCoreRender();
    if (coreRender == nullptr) {
        coreRender = new QFractalRenderer(QMF);
        renderWindow->SetView(coreRender->getImagePtr());
        QObject::connect(coreRender, SIGNAL(progress(int,int)), mainWindow, SLOT(requestUpdateProgressBar(int,int)));
        QObject::connect(coreRender, SIGNAL(progress(int,int)), renderWindow, SLOT(RequestUpdate()));
    } else {
        coreRender->stop();
        coreRender->setSettings(QMF);
    }
    coreRender->start();
}

void FractUIManager::stopCoreRender() {
    if (coreRender != nullptr) {
        coreRender->stop();
        //delete coreRender;
        //coreRender = nullptr;
    }
}
