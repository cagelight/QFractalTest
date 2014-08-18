#ifndef FRACTAL_SETTINGS_H
#define FRACTAL_SETTINGS_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef struct coord2 {
    float X;
    float Y;
} coord2;
const coord2 coord2DEF = {0.0f,0.0f};

typedef struct ARGB { unsigned char A, R, G, B; } ARGB;
typedef struct BGRA { unsigned char B, G, R, A; } BGRA;

typedef BGRA CColor;
const CColor CColorBLACK = {0x00,0x00,0x00,0xFF};
const CColor CColorWHITE = {0xFF,0xFF,0xFF,0xFF};

typedef struct FractSettings {
    unsigned int Width;
    unsigned int Height;
    float Scale;
    coord2 Offset;
    unsigned int Iterations;
} FractSettings;

#endif // FRACTAL_SETTINGS_H
