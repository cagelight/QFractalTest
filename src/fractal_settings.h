#ifndef FRACTAL_SETTINGS_H
#define FRACTAL_SETTINGS_H

#include "render_enumerations.h"

#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef struct coord2 {
    float X;
    float Y;
} coord2;
const extern coord2 coord2DEF;

typedef struct ARGB { unsigned char A, R, G, B; } ARGB;
typedef struct BGRA { unsigned char B, G, R, A; } BGRA;

typedef BGRA CColor;
const extern CColor CColorBLACK;
const extern CColor CColorWHITE;

typedef struct r2pass {
    unsigned int funcSize;
    R2DFUNC *funcs;
    R2DSTOP stop;
} r2pass;

typedef struct fract_settings { //Use only the below functions for creation/destruction, and don't forget to allocate "colorbake" yourself.
    unsigned int Width;
    unsigned int Height;
    float Scale;
    coord2 Offset;
    unsigned int Iterations;
    CColor* colorbake;
    r2pass pass;
} fract_settings;

#endif // FRACTAL_SETTINGS_H
