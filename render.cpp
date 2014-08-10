#include "render.hpp"
#include "ui.hpp"
#include "debug.hpp"

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
        rowPtr = (unsigned int*)renderImage->scanLine(rowCur);
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

inline static unsigned int render_pixel(const FractSettings& s, const glm::vec2& coords) {
    glm::vec2 pre = glm::vec2(0, 0);
    glm::vec2 post = glm::vec2(0, 0);
    unsigned int iter = 0;
    for (;iter < s.Iterations; iter++) {
        pre.x = (post.x * post.x - post.y * post.y) + coords.x;
        pre.y = (post.y * post.x + post.x * post.y) + coords.y;
        if ((pre.x * pre.x + pre.y * pre.y) > 1e5f) break;
        post.x = pre.x;
        post.y = pre.y;
    }
    return iter-1;
}

static void render_line(const FractSettings& s, float fhpos, unsigned int *rowPtr, const Color* colorbake) {
    glm::vec2 coords(0.0f, fhpos * s.Scale - s.Offset.y);
    for (unsigned int c = 0; c < s.Width; c++) {
        coords.x = (c / (float)s.Width - 0.5) * s.Scale - s.Offset.x;
        rowPtr[c] = colorbake[render_pixel(s, coords)];
    }
}

static void render_image(FractSettings s, Color* iterbake) {
    unsigned int rowNum;
    unsigned int* rowPtr;
    while (delegate_line(rowNum, rowPtr) && mainGreenlight) {
        float fheight = (rowNum / (float)s.Height - 0.5);
        render_line(s, fheight, rowPtr, iterbake);
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

static void begin_threaded_render(FractSettings s) {
    writeLock.lock();
    pointerLock.lock();
    delete renderImage;
    renderImage = new QImage(s.Width, s.Height, QImage::Format_RGB32);
    renderImage->fill(0x00000000);
    pointerLock.unlock();
    mainGreenlight = true;
    setup_delegate(s.Height);
    renderStart = std::chrono::high_resolution_clock::now();
    lastProgressUpdate = update_progress_interval::zero();
    lastImageUpdate = update_image_interval::zero();
    progress.setMax((unsigned long)s.Width * s.Height);
    progress.reset();
    Color* iterbake = s.Gradient.Bake(s.Iterations);
    for (unsigned int i = 0; i < numCores; i++) {
        renderThreads.push(std::thread(std::bind(render_image, s, iterbake)));
    }
    while (renderThreads.size() > 0) {
        std::thread &t = renderThreads.front();
        if (t.joinable())
            t.join();
        renderThreads.pop();
    }
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

static void begin_render(FractSettings s) {
    stop_threaded_render();
    renderThread = new std::thread(std::bind(begin_threaded_render, s));
}
///THREADED RENDER BLOCK/// END

void render::initialize() {
    numCores = std::thread::hardware_concurrency();
    if (numCores == 0) numCores = 1;
    renderImage = new QImage(1, 1, QImage::Format_RGB32);
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

void render::render(FractSettings settings) {
    begin_render(settings);
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
