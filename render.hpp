#ifndef RENDER_HPP
#define RENDER_HPP

#include <QImage>

#include "fractal.hpp"

namespace render {
    void initialize();
    QImage get_scaled_copy(int, int);
    QImage get_image_copy();
    void render(fractal::Settings);
}

#endif // RENDER_HPP
