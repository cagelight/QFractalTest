#include "render.hpp"
#include "ui.hpp"
#include "debug.hpp"
#include "multigradient.hpp"

#include <chrono>
#include <ctgmath>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

#include <glm/glm.hpp>

static unsigned int numCores = 0;

static QImage* renderImage;
static std::mutex pointerLock;
static std::mutex writeLock;
static std::mutex progressLock;

///THREADED RENDER BLOCK/// BEGIN

static bool mainGreenlight;
static std::thread* renderThread;
static RenderProgress progress;
static std::chrono::high_resolution_clock::time_point renderStart;
typedef std::chrono::duration<int, std::ratio<1, 100>> update_progress_interval;
typedef std::chrono::duration<int, std::ratio<1, 10>> update_image_interval;
static update_progress_interval lastProgressUpdate;
static update_image_interval lastImageUpdate;
static std::mutex chronoLock;

static std::queue<std::thread> renderThreads;

static std::mutex delegateLock;
static unsigned int rowCur = 0;
static unsigned int rowNum = 0;
static void setup_delegate(unsigned int rows) {
    rowCur = 0;
    rowNum = rows;
}

static bool delegate_line(unsigned int &lineNum, unsigned int*& rowPtr) {
    delegateLock.lock();
    if (rowCur != rowNum) {
        lineNum = rowCur;
        rowPtr = (unsigned int*)(QRgb*)renderImage->scanLine(rowCur);
        rowCur++;
        delegateLock.unlock();
        return true;
    } else {
        lineNum = -1;
        rowPtr = nullptr;
        delegateLock.unlock();
        return false;
    }

}

static void render_image(FractSettings s, CColor* iterbake, r2pass r2p) {
    unsigned int rowNum;
    unsigned int* rowPtr;
    while (delegate_line(rowNum, rowPtr) && mainGreenlight) {
        float fheight = (rowNum / (float)s.Height - 0.5);
        //render_line(s, fheight, rowPtr, iterbake);
        render2d_line(s, fheight, (CColor*)rowPtr, iterbake, r2p);
        progressLock.lock();
        progress.update(s.Width);
        progressLock.unlock();
        if(chronoLock.try_lock()) {
            update_progress_interval newProgressUpdate = std::chrono::duration_cast<update_progress_interval>(std::chrono::high_resolution_clock::now() - renderStart);
            if (lastProgressUpdate.count() < newProgressUpdate.count()) {ui::update_render_progress();}
            lastProgressUpdate = newProgressUpdate;
            update_image_interval newImageUpdate = std::chrono::duration_cast<update_image_interval>(std::chrono::high_resolution_clock::now() - renderStart);
            if (lastImageUpdate.count() < newImageUpdate.count()) {ui::update_render_view();}
            lastImageUpdate = newImageUpdate;
            chronoLock.unlock();
        }
    }
}

static void begin_threaded_render(Fract s) {
    writeLock.lock();
    pointerLock.lock();
    delete renderImage;
    renderImage = new QImage(s.Settings.Width, s.Settings.Height, QImage::Format_RGB32);
    renderImage->fill(0x00000000);
    pointerLock.unlock();
    mainGreenlight = true;
    setup_delegate(s.Settings.Height);
    renderStart = std::chrono::high_resolution_clock::now();
    lastProgressUpdate = update_progress_interval::zero();
    lastImageUpdate = update_image_interval::zero();
    progress.setMax((unsigned long)s.Settings.Width * s.Settings.Height);
    progress.reset();
    CColor* iterbake = (CColor*)s.Gradient.Bake(s.Settings.Iterations);
    r2pass r2p = create_pass(2);
    r2p.funcs[1] = FUNC_MANDELBROT;
    r2p.funcs[0] = FUNC_FEATHERBOOT;
    r2p.stop = STOP_MANDELBROT;
    for (unsigned int i = 0; i < numCores; i++) {
        renderThreads.push(std::thread(std::bind(render_image, s.Settings, iterbake, r2p)));
    }
    while (renderThreads.size() > 0) {
        std::thread &t = renderThreads.front();
        if (t.joinable())
            t.join();
        renderThreads.pop();
    }
    destroy_pass(r2p);
    ui::update_render_progress();
    ui::update_render_view();
    delete[] iterbake;
    mainGreenlight = false;
    writeLock.unlock();
}

static void stop_threaded_render() {
    mainGreenlight = false;
    if (renderThread != nullptr && renderThread->joinable()) {
        renderThread->join();
        delete renderThread;
        renderThread = nullptr;
    } else if (renderThread != nullptr) {
        delete renderThread;
        renderThread = nullptr;
    }
}

static void begin_render(Fract s) {
    stop_threaded_render();
    renderThread = new std::thread(std::bind(begin_threaded_render, s));
}
///THREADED RENDER BLOCK/// END

void render::initialize() {
    numCores = std::thread::hardware_concurrency();
    if (numCores == 0) numCores = 1;
    renderImage = new QImage(1, 1, QImage::Format_RGB32);
    renderImage->fill(0xFF000000);
}

QImage render::get_scaled_copy(int width, int height) {
    pointerLock.lock();
    QImage Qi = renderImage->scaled(width, height, Qt::KeepAspectRatio);
    pointerLock.unlock();
    return Qi;
}

QImage render::get_image_copy() {
    pointerLock.lock();
    QImage Qi = renderImage->copy(renderImage->rect());
    pointerLock.unlock();
    return Qi;
}

void render::save_render(const QString& path) {
    pointerLock.lock();
    renderImage->save(path);
    pointerLock.unlock();
}

void render::render(Fract f) {
    begin_render(f);
}

void render::stop_render() {
    stop_threaded_render();
}

RenderProgress render::get_progress() {
    progressLock.lock();
    auto RP = progress;
    progressLock.unlock();
    return RP;
}

void render::terminate() {
    if (renderThread != nullptr && renderThread->joinable()) {
        mainGreenlight = false;
        renderThread->join();
    }
    delete renderImage;
    delete renderThread;
}
