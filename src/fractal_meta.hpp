#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glm/glm.hpp>

#include "common.hpp"
#include "multigradient.hpp"

extern "C" {
    #include "fractal_settings.h"
}

struct Fract {
    FractSettings Settings;
    MultiGradient Gradient;
};

#endif // SETTINGS_HPP
