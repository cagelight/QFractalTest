#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>

#include <QSize>
#include <QPoint>

#include "common.hpp"
#include "multigradient.hpp"

extern "C" {
    #include "render_enumerations.h"
}

class fract_pass : public std::vector<R2DFUNC> {
public:
    fract_pass(R2DSTOP stop, std::initializer_list<R2DFUNC> func) : vector<R2DFUNC>(func), stop(stop) {}
private:
    R2DSTOP stop;
};

class QFractalMeta {
public:
    QFractalMeta(QSize size, float scale, QPointF offset, unsigned int iterations, MultiGradient gradient, fract_pass pass):
        size(size),
        scale(scale),
        offset(offset),
        iterations(iterations),
        gradient(gradient),
        pass(pass)
    {}
    QFractalMeta(int width, int height, float scale, float offsetx, float offsety, unsigned int iterations, MultiGradient gradient, fract_pass pass):
        size(width, height),
        scale(scale),
        offset(offsetx, offsety),
        iterations(iterations),
        gradient(gradient),
        pass(pass)
    {}
    QSize size;
    float scale;
    QPointF offset;
    unsigned int iterations;
    MultiGradient gradient;
    fract_pass pass;
};

#endif // SETTINGS_HPP
