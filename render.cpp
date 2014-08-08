#include "render.hpp"
#include "ui.hpp"
#include "debug.hpp"

#include <chrono>
#include <ctgmath>
#include <functional>
#include <mutex>
#include <thread>

#include <glm/glm.hpp>

static QImage* renderImage;
static std::mutex pointerLock;
static std::mutex writeLock;
static std::mutex progressLock;

///THREADED RENDER BLOCK/// BEGIN

static bool renderThreadGreenlight;
static std::thread* renderThread;
static RenderProgress progress;
static std::chrono::high_resolution_clock::time_point renderStart;
typedef std::chrono::duration<int, std::ratio<1, 100>> update_progress_interval;
typedef std::chrono::duration<int, std::ratio<1, 10>> update_image_interval;
static update_progress_interval lastProgressUpdate;
static update_image_interval lastImageUpdate;

static void render_image(fractal::Settings settings, QImage* img) {
    renderStart = std::chrono::high_resolution_clock::now();
    lastProgressUpdate = update_progress_interval::zero();
    lastImageUpdate = update_image_interval::zero();
    progress.setMax((unsigned long)img->width() * img->height());
    progress.reset();
    glm::vec2 coords;
    glm::vec2 pre, post;
    Color* iterbake = settings.Gradient.Bake(settings.Iterations);
    for (int r = 0; r < img->height(); r++) {
        if (!renderThreadGreenlight) break;
        coords.y = (r / (float)img->height() - 0.5) * settings.Scale - settings.Offset.y;
        unsigned int* rowPtr = (uint*)img->scanLine(r);
        for (int c = 0; c < img->width(); c++) {
            coords.x = (c / (float)img->width() - 0.5) * settings.Scale - settings.Offset.x;
            pre = glm::vec2(0, 0);
            post = glm::vec2(0, 0);
            unsigned int iter = 0;
            for (;iter < settings.Iterations; iter++) {
                pre.x = (post.x * post.x - post.y * post.y) + coords.x;
                pre.y = (post.y * post.x + post.x * post.y) + coords.y;
                if ((pre.x * pre.x + pre.y * pre.y) > 1e5f) break;
                post.x = pre.x;
                post.y = pre.y;
            }
            rowPtr[c] = iterbake[iter-1];
        }
        progressLock.lock();
        progress.update(img->width());
        progressLock.unlock();
        update_progress_interval newProgressUpdate = std::chrono::duration_cast<update_progress_interval>(std::chrono::high_resolution_clock::now() - renderStart);
        if (lastProgressUpdate.count() < newProgressUpdate.count() || progress.totalProgress() == 1.0f) {ui::update_render_progress();}
        lastProgressUpdate = newProgressUpdate;
        update_image_interval newImageUpdate = std::chrono::duration_cast<update_image_interval>(std::chrono::high_resolution_clock::now() - renderStart);
        if (lastImageUpdate.count() < newImageUpdate.count() || progress.totalProgress() == 1.0f) {ui::update_render_view();}
        lastImageUpdate = newImageUpdate;
    }
    delete[] iterbake;
    renderThreadGreenlight = false;
    ui::update_render_progress();
    ui::update_render_view();
}

static void begin_threaded_render(fractal::Settings s) {
    writeLock.lock();
    pointerLock.lock();
    delete renderImage;
    renderImage = new QImage(s.Width, s.Height, QImage::Format_RGB32);
    renderImage->fill(0x00000000);
    pointerLock.unlock();
    render_image(s, renderImage);
    ui::update_render_view();
    writeLock.unlock();
}

static void begin_render(fractal::Settings s) {
    if (renderThread != nullptr && renderThread->joinable()) {
        renderThreadGreenlight = false;
        renderThread->join();
        delete renderThread;
    } else if (renderThread != nullptr) {
        delete renderThread;
    }
    renderThreadGreenlight = true;
    renderThread = new std::thread(std::bind(begin_threaded_render, s));
}
///THREADED RENDER BLOCK/// END

void render::initialize() {
    renderImage = new QImage(1, 1, QImage::Format_RGB32);
    renderImage->setPixel(0, 0, 0xff000000);
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

QImage render::render_preview(fractal::Settings settings) {
    settings.Iterations = 10;
    settings.Width = 128;
    settings.Height = 128;
    QImage preview(128, 128, QImage::Format_RGB32);
    render_image(settings, &preview);
    return preview;
}

void render::render(fractal::Settings settings) {
    begin_render(settings);
}

RenderProgress render::get_progress() {
    progressLock.lock();
    auto RP = progress;
    progressLock.unlock();
    return RP;
}

void render::terminate() {
    if (renderThread != nullptr && renderThread->joinable()) {
        renderThreadGreenlight = false;
        renderThread->join();
    }
    delete renderImage;
    delete renderThread;
}
