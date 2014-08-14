#ifndef FRACTAL_SETTINGS_H
#define FRACTAL_SETTINGS_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#define LITTLE_ENDIAN_PRIORITY

extern bool little_endian;

typedef struct coord2 {
    float X;
    float Y;
} coord2;
const coord2 coord2DEF = {0.0f,0.0f};

#ifdef LITTLE_ENDIAN_PRIORITY

typedef struct CColor {
    unsigned char B, G, R, A;
} CColor;
const CColor CColorBLACK = {0x00,0x00,0x00,0xFF};
const CColor CColorWHITE = {0xFF,0xFF,0xFF,0xFF};

#else

typedef struct CColor {
    unsigned char A, R, G, B;
} CColor;
const CColor CColorBLACK = {0xFF,0x00,0x00,0x00};
const CColor CColorWHITE = {0xFF,0xFF,0xFF,0xFF};

#endif

typedef struct FractSettings {
    unsigned int Width;
    unsigned int Height;
    float Scale;
    coord2 Offset;
    unsigned short Iterations;
} FractSettings;

#endif // FRACTAL_SETTINGS_H
