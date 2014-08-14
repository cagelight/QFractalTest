#ifndef RENDER_PIXEL_H
#define RENDER_PIXEL_H

/*
 * C was chosen to be used for pixel render functions for maximum parallel development efficiency between CPU and GPU (OpenCL).
 */

#include "fractal_settings.h"

void render2d_line(FractSettings FS, float linePos, CColor* rowPtr, CColor* colorBake);

const int render2d_mandelbrot_pixel_tag = 0xABCD1001;
unsigned int render2d_mandelbrot_pixel(unsigned int maxiter, float x, float y);

#endif // RENDER_PIXEL_H
