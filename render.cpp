#include "render.hpp"
#include "ui.hpp"
#include "debug.hpp"

#include <ctgmath>
#include <mutex>

#include <glm/glm.hpp>

static QImage* renderImage;
static std::mutex pointerLock;

static void render_image(fractal::Settings settings, QImage* img) {
    glm::vec2 coords;
    glm::vec2 pre, post;
    Color* iterbake = settings.Gradient.Bake(settings.Iterations);
    for (int r = 0; r < img->height(); r++) {
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
    }
    delete[] iterbake;
}

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
    pointerLock.lock();
    delete renderImage;
    renderImage = new QImage(settings.Width, settings.Height, QImage::Format_RGB32);
    pointerLock.unlock();
    render_image(settings, renderImage);
    ui::update_render_view();
}
