#ifndef RENDER_PIXEL_H
#define RENDER_PIXEL_H

#include <stdlib.h>
#include "fractal_settings.h"

void render2d_line(fract_settings fs, CColor* rowPtr, unsigned int linePos);

#endif // RENDER_PIXEL_H
