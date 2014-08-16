#ifndef RENDER_PIXEL_H
#define RENDER_PIXEL_H

#include <stdlib.h>
#include "fractal_settings.h"

typedef struct dataset {
    unsigned int size;
    float *data;
} dataset;
extern const char* cl_dataset; //OPENCL COMPLIMENT

typedef void (*r2func) (float, float, dataset*);
typedef bool (*r2stop) (dataset*);
typedef struct r2funcset {
    r2func function;
    const char *cl_text;
    unsigned int dsize;
} r2funcset;
typedef struct r2stopset {
    r2stop function;
    const char *cl_text;
    unsigned int dsize;
} r2stopset;

dataset create_dataset(unsigned int);
void destroy_dataset(dataset);

void render2d_line(FractSettings FS, float linePos, CColor* rowPtr, CColor* colorBake);

//ITERATION FUNCTIONS

const unsigned int R2_MNDL_M0 = 0x000A0004;
void r2_mandelbrot(float x, float y, dataset *d);
extern const char* cl_r2_mandelbrot; //OPENCL COMPLIMENT

const unsigned int R2_MNDL_M1 = 0x000A0104;
void r2_mandelbrot_mod1(float x, float y, dataset *d);
extern const char* cl_r2_mandelbrot_mod1; //OPENCL COMPLIMENT

//STOP CONDITION FUNCTIONS

const unsigned int R2_MNDL_STOP = 0x010A0004;
bool r2_mandelbrot_stop(dataset* d);
extern const char* cl_r2_mandelbrot_stop; //OPENCL COMPLIMENT

typedef enum R2DFUNC {
    Mandelbrot = R2_MNDL_M0,
    MandelbrotMod1 = R2_MNDL_M1
} R2DFUNC;

typedef enum R2DSTOP {
    MandelbrotSTOP = R2_MNDL_STOP,
} R2DSTOP;

typedef struct r2pass {
    unsigned int funcSize;
    R2DFUNC *funcs;
    R2DSTOP stop;
} r2pass;
typedef struct r2set {
    unsigned int funcSize;
    r2funcset *funcs;
    r2stopset stop;
    unsigned int dsize;
} r2set;

r2funcset get_function(R2DFUNC R2D);
r2stopset get_stop(R2DSTOP R2D);

r2set get_set(r2pass);

void destroy_pass(r2pass);
void destroy_set(r2set);

#endif // RENDER_PIXEL_H
