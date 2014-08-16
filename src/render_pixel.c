#include "render_pixel.h"

const char* cl_dataset = "typedef struct dataset { unsigned int size; float *data; } dataset;";

const float cutoff = 1e5f;

dataset create_dataset(unsigned int size) {
    dataset d;
    d.size = size;
    d.data = calloc(size, sizeof(float));
    return d;
}

void destroy_dataset(dataset d) {
    free (d.data);
}

void render2d_line(FractSettings FS, float linePos, CColor* rowPtr, CColor* colorBake) {
    coord2 coords = coord2DEF;
    coords.Y = linePos * FS.Scale - FS.Offset.Y;
    for (unsigned int c = 0; c < FS.Width; c++) {
        coords.X = (c / (float)FS.Width - 0.5) * FS.Scale - FS.Offset.X;
        unsigned int iter = 0;
        dataset rd = create_dataset(4);
        do {
            r2_mandelbrot(coords.X, coords.Y, &rd);
            iter++;
        } while (!r2_mandelbrot_stop(&rd) && iter < FS.Iterations);
        rowPtr[c] = colorBake[iter-1];
        destroy_dataset(rd);
    }
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

void r2_mandelbrot_mod1(float x, float y, dataset *d) {
    d->data[0] = (d->data[2] * d->data[2] - d->data[3]) + x;
    d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;
    d->data[2] = d->data[0];
    d->data[3] = d->data[1];
} const char *cl_r2_mandelbrot_mod1 =
        "d->data[0] = (d->data[2] * d->data[2] - d->data[3]) + x;"
        "d->data[1] = (d->data[3] * d->data[2] + d->data[2] * d->data[3]) + y;"
        "d->data[2] = d->data[0];"
        "d->data[3] = d->data[1];"
        ;

//--------------------STOP FUNCTIONS--------------------

bool r2_mandelbrot_stop(dataset *d) {
    if (d->data[0] * d->data[0] + d->data[1] * d->data[1] > cutoff)
        return true; return false;
} const char *cl_r2_mandelbrot_stop =
        "if (d->data[0] * d->data[0] + d->data[1] * d->data[1] > cutoff)"
        "return true; return false;"
        ;

//----------------------------------------

r2funcset get_function(R2DFUNC R2D) {
    switch (R2D) {
    case Mandelbrot:
        return (r2funcset){&r2_mandelbrot, cl_r2_mandelbrot, 4};
    case MandelbrotMod1:
        return (r2funcset){&r2_mandelbrot_mod1, cl_r2_mandelbrot_mod1, 4};
    default:
        return (r2funcset){&r2_mandelbrot, cl_r2_mandelbrot, 4};
    }
}

r2stopset get_stop(R2DSTOP R2D) {
    switch (R2D) {
    case MandelbrotSTOP:
        return (r2stopset){&r2_mandelbrot_stop, cl_r2_mandelbrot_stop, 4};
    default:
        return (r2stopset){&r2_mandelbrot_stop, cl_r2_mandelbrot_stop, 4};
    }
}

r2set get_set(r2pass rp) {
    r2set rs;
    rs.funcSize = rp.funcSize;
    rs.stop = get_stop(rp.stop);
    rs.funcs = malloc(sizeof(rs.funcs)*rs.funcSize);
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

void destroy_pass(r2pass rp) {
    free(rp.funcs);
}

void destroy_set(r2set rs) {
    free(rs.funcs);
}
