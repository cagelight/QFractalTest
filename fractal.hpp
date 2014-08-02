#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glm/glm.hpp>

#include "common.hpp"

namespace fractal {

struct Settings {
    unsigned int Width = 512;
    unsigned int Height = 512;
    float Scale = 1.0f;
    glm::vec2 Offset = glm::vec2(0, 0);
    unsigned int Iterations;
    MultiGradient Gradient;
};

}

#endif // SETTINGS_HPP
