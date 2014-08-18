#ifndef RENDER_PIXEL_H
#define RENDER_PIXEL_H

#include <stdlib.h>
#include "fractal_settings.h"

typedef enum R2DFUNC {
    FUNC_MANDELBROT,
    FUNC_MANDELBRIM,
    FUNC_FEATHERBOOT,
} R2DFUNC;

typedef enum R2DSTOP {
    STOP_MANDELBROT,
    STOP_PAVILION,
    STOP_CRYSTALINE,
} R2DSTOP;

typedef struct r2pass {
    unsigned int funcSize;
    R2DFUNC *funcs;
    R2DSTOP stop;
} r2pass;

r2pass create_pass(unsigned int size);
void destroy_pass(r2pass);

void render2d_line(FractSettings FS, float linePos, CColor* rowPtr, CColor* colorBake, r2pass);

#endif // RENDER_PIXEL_H
