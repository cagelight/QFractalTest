#include "render_pixel.h"

#include <math.h>

typedef struct dataset {
    unsigned int size;
    float *data;
} dataset;
const char* cl_dataset = "typedef struct dataset { unsigned int size; float *data; } dataset;";

dataset create_dataset(unsigned int size) {
    dataset d;
    d.size = size;
    d.data = calloc(size, sizeof(float));
    return d;
}

void destroy_dataset(dataset d) {
    free (d.data);
}

typedef void (*r2func) (float, float, dataset*);
typedef bool (*r2stop) (dataset*);
typedef struct r2funcset {
    r2func func;
    unsigned int dsize;
} r2funcset;
typedef struct r2stopset {
    r2stop func;
    unsigned int dsize;
} r2stopset;

typedef struct r2set {
    unsigned int funcSize;
    r2funcset *funcs;
    r2stopset stop;
    unsigned int dsize;
} r2set;

const float cutoff = 4.0f;

r2funcset get_function(R2DFUNC R2D);
r2stopset get_stop(R2DSTOP R2D);

r2pass create_pass(unsigned int size) {
    r2pass r2p;
    r2p.funcSize = size;
    r2p.funcs = malloc(sizeof(R2DFUNC)*size);
    return r2p;
}

void destroy_pass(r2pass rp) {
    free(rp.funcs);
}

r2set create_set(r2pass rp) {
    r2set rs;
    rs.funcSize = rp.funcSize;
    rs.stop = get_stop(rp.stop);
    rs.funcs = malloc(sizeof(r2funcset)*rs.funcSize);
    rs.dsize = 0;
    for (unsigned int i = 0; i < rp.funcSize; i++) {
        rs.funcs[i] = get_function(rp.funcs[i]);
        unsigned int curdsize = rs.funcs[i].dsize;
        if (curdsize > rs.dsize) {
            rs.dsize = curdsize;
        }
    }
    return rs;
}

void destroy_set(r2set rs) {
    free(rs.funcs);
}



//--------------------ITERATION FUNCTIONS--------------------

void r2_mandelbrot(float x, float y, dataset *d) {
    d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3]) + x;
    d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;
    d->data[2] = d->data[0];
    d->data[3] = d->data[1];
} const char *cl_r2_mandelbrot =
        "d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3]) + x;"
        "d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;"
        "d->data[2] = d->data[0];"
        "d->data[3] = d->data[1];";

void r2_mandelbrim(float x, float y, dataset *d) {
    d->data[0] = (d->data[2] * d->data[2] - d->data[3]) + x;
    d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;
    d->data[2] = d->data[0];
    d->data[3] = d->data[1];
} const char *cl_r2_mandelbrim =
        "d->data[0] = (d->data[2] * d->data[2] - d->data[3]) + x;"
        "d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;"
        "d->data[2] = d->data[0];"
        "d->data[3] = d->data[1];"
        ;

void r2_featherboot(float x, float y, dataset *d) {
    d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3] + d->data[4]) + x;
    d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3] - d->data[4]) + y;
    d->data[2] = d->data[0];
    d->data[3] = d->data[1];
    d->data[4] = d->data[0] - d->data[1];
} const char *cl_r2_featherboot =
        "d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3] + d->data[4]) + x;"
        "d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3] - d->data[4]) + y;"
        "d->data[2] = d->data[0];"
        "d->data[3] = d->data[1];"
        "d->data[4] = d->data[0] - d->data[1];"
        ;

void r2_burningship(float x, float y, dataset *d) {
    d->data[2] = fabs(d->data[2]);
    d->data[3] = fabs(d->data[3]);
    d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3]) + x;
    d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;
    d->data[2] = d->data[0];
    d->data[3] = d->data[1];
} const char *cl_r2_burningship =
        "d->data[2] = fabs(d->data[2]);"
        "d->data[3] = fabs(d->data[3]);"
        "d->data[0] = (d->data[2] * d->data[2] - d->data[3] * d->data[3]) + x;"
        "d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;"
        "d->data[2] = d->data[0];"
        "d->data[3] = d->data[1];";


//--------------------STOP FUNCTIONS--------------------

bool r2_mandelbrot_stop(dataset *d) {
    if (d->data[0] * d->data[0] + d->data[1] * d->data[1] > cutoff)
        return true; return false;
} const char *cl_r2_mandelbrot_stop =
        "if (d->data[0] * d->data[0] + d->data[1] * d->data[1] > cutoff)"
        "return true; return false;"
        ;

bool r2_pavilion_stop(dataset *d) {
    if (d->data[0] * d->data[0] + d->data[1] > cutoff)
        return true; return false;
} const char *cl_r2_pavilion_stop =
        "if (d->data[0] * d->data[0] + d->data[1] > cutoff)"
        "return true; return false;"
        ;

bool r2_rave_stop(dataset *d) {
    if (d->data[0] + d->data[1] > cutoff)
        return true; return false;
} const char *cl_r2_rave_stop =
        "if (d->data[0] + d->data[1] > cutoff)"
        "return true; return false;"
        ;




//----------------------------------------


r2funcset get_function(R2DFUNC R2D) {
    switch (R2D) {
DEFAULT:case FUNC_MANDELBROT:
        return (r2funcset){&r2_mandelbrot, 4};
    case FUNC_MANDELBRIM:
        return (r2funcset){&r2_mandelbrim, 4};
    case FUNC_FEATHERBOOT:
        return (r2funcset){&r2_featherboot, 5};
    case FUNC_BURNINGSHIP:
        return (r2funcset){&r2_burningship, 4};
    default:
        goto DEFAULT;
    }
}

r2stopset get_stop(R2DSTOP R2D) {
    switch (R2D) {
DEFAULT:case STOP_MANDELBROT:
        return (r2stopset){&r2_mandelbrot_stop, 4};
    case STOP_PAVILION:
        return (r2stopset){&r2_pavilion_stop, 4};
    case STOP_CRYSTALINE:
        return (r2stopset){&r2_rave_stop, 4};
    default:
        goto DEFAULT;
    }
}

unsigned int render2d_pixel(coord2 c, r2set rs, unsigned int maxiter) {
    dataset d = create_dataset(rs.dsize);
    unsigned int iter = 0;
    do {
        rs.funcs[iter % rs.funcSize].func(c.X, c.Y, &d);
        iter++;
    } while (!rs.stop.func(&d) && iter < maxiter);
    destroy_dataset(d);
    return iter-1;
}

void render2d_line(const fract_settings *fs, CColor* rowPtr, unsigned int linePos) {
    r2set rs = create_set(fs->pass);
    coord2 coords = coord2DEF;
    float fheight = (linePos / (float)fs->Height - 0.5);
    coords.Y = fheight * fs->Scale - fs->Offset.Y;
    for (unsigned int c = 0; c < fs->Width; c++) {
        coords.X = (c / (float)fs->Width - 0.5) * fs->Scale - fs->Offset.X;
        rowPtr[c] = fs->colorbake[render2d_pixel(coords, rs, fs->Iterations)];;
    }
    destroy_set(rs);
}
