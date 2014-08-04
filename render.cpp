#include "render.hpp"
#include "ui.hpp"
#include "debug.hpp"

#include <ctgmath>
#include <mutex>

#include <glm/glm.hpp>

static QImage* renderImage;

static std::mutex pointerLock;

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

void render::render(fractal::Settings settings) {
    pointerLock.lock();
    delete renderImage;
    renderImage = new QImage(settings.Width, settings.Height, QImage::Format_RGB32);
    pointerLock.unlock();
    glm::vec2 coords;
    glm::vec2 pre, post;

    Color* iterbake = settings.Gradient.Bake(settings.Iterations);

    for (int r = 0; r < renderImage->height(); r++) {
        coords.y = (r / (float)renderImage->height() - 0.5) * settings.Scale - settings.Offset.y;
        unsigned int* rowPtr = (uint*)renderImage->scanLine(r);
        for (int c = 0; c < renderImage->width(); c++) {
            coords.x = (c / (float)renderImage->width() - 0.5) * settings.Scale - settings.Offset.x;
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
            Range MM = settings.Gradient.GetRange();
            rowPtr[c] = iterbake[iter-1];
        }
    }
    delete[] iterbake;
    ui::update_render_view();
}
