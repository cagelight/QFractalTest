#include "qobject_fractalrenderer.hpp"

extern "C" {
    #include "fractal_settings.h"
    #include "render_pixel.h"
}

static CColor* convert_qcolorptr_ccolorptr(QColor* ptr, unsigned int size) { //DELETES THE ORIGINAL
    CColor *newC = new CColor[size];
    for (unsigned int i = 0; i < size; i++) {
        unsigned int rgba = ptr[i].rgba();
        newC[i] = *(CColor*)&rgba;
    }
    delete[] ptr;
    return newC;
}

QFractalRenderer::QFractalRenderer(QFractalMeta F) : QObject(0), fract(F), image(F.size.width(), F.size.height(), QImage::Format_ARGB32) {
    image.fill(0x00000000);
    dopause = false;
    dostop = false;
    isactive = false;
    this->setupFract();
}

QImage QFractalRenderer::getImage() {
    return image;
}

void QFractalRenderer::setSettings(QFractalMeta F) {
    this->stop();
    this->fract = F;
    this->reset();
}

void QFractalRenderer::setupFract() {
    if (fractC != nullptr) {
        delete[] fractC->pass.funcs;
        delete[] fractC->colorbake;
        delete fractC;
    }
    fractC = new fract_settings();
    fractC->Width = fract.size.width();
    fractC->Height = fract.size.height();
    fractC->Iterations = fract.iterations;
    fractC->Offset = (coord2) {(float)fract.offset.x(), (float)fract.offset.y()};
    fractC->Scale = fract.scale;
    fractC->pass.funcs = new R2DFUNC[1];
    fractC->pass.funcs[0] = FUNC_MANDELBROT;
    fractC->pass.stop = STOP_MANDELBROT;
    fractC->pass.funcSize = 1;
    fractC->colorbake = convert_qcolorptr_ccolorptr(fract.gradient.Bake(fract.iterations), fract.iterations);
}

void QFractalRenderer::start() {
    if (isactive || dostop) {
        this->stop();
    }
    if (dopause) {
        this->resume();
    } else {
        dopause = false;
        dostop = false;
        isactive = true;
        delegateThread = new std::thread(&QFractalRenderer::delegate, this);
    }
}

void QFractalRenderer::stop() {
    if (isactive) {
        dostop = true;
        if (dopause) this->resume();
        this->completeDelegate();
        dostop = false;
        isactive = false;
    }
}

void QFractalRenderer::pause() {
    if (isactive && !dopause && !dostop) {
        dopause = true;
        isactive = false;
    }
}

void QFractalRenderer::resume() {
    dopause = false;
    isactive = true;
}

void QFractalRenderer::reset() {
    this->stop();
    image = QImage(fract.size.width(), fract.size.height(), QImage::Format_ARGB32);
    image.fill(0x00000000);
    this->setupFract();
}

void QFractalRenderer::completeDelegate() {
    if (delegateThread != nullptr) {
        if (delegateThread->joinable()) {
            delegateThread->join();
            delete delegateThread;
        }
        delegateThread = nullptr;
    }
}

void QFractalRenderer::delegate() {
    unsigned int numCores = std::thread::hardware_concurrency();
    delegateCur = 0;
    for (unsigned int i = 0; i < numCores; i++) {
        workerThreads.push(std::thread(&QFractalRenderer::work, this));
    }
    while (workerThreads.size() > 0) {
        std::thread &t = workerThreads.front();
        if (t.joinable())
            t.join();
        workerThreads.pop();
    }
    emit finished();
}

void QFractalRenderer::work() {
    unsigned int rowNum;
    unsigned int* rowPtr;
    while (delegateLine(rowNum, rowPtr) && !dostop) {
        render2d_line(*fractC, (CColor*)rowPtr, rowNum);
        emit progressUpdate(fract.size.height(), delegateCur);
    }
}

bool QFractalRenderer::delegateLine(unsigned int &lineNum, unsigned int*& rowPtr) {
    delegateLock.lock();
    if (delegateCur != fract.size.height() && !dostop) {
        lineNum = delegateCur;
        rowPtr = (unsigned int*)image.scanLine(delegateCur);
        delegateCur++;
        delegateLock.unlock();
        return true;
    } else {
        lineNum = -1;
        rowPtr = nullptr;
        delegateLock.unlock();
        return false;
    }

}
