#ifndef RENDER_PIXEL_H
#define RENDER_PIXEL_H

/*
 * C was chosen to be used for pixel render functions for maximum parallel development efficiency between CPU and GPU (OpenCL).
 */

typedef struct coord2 {
    float X;
    float Y;
} coord2;

unsigned int render2d_mandelbrot_pixel(unsigned int maxiter, float x, float y);

#endif // RENDER_PIXEL_H
