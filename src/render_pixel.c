#include "render_pixel.h"

bool little_endian;

void render2d_line(FractSettings FS, float linePos, CColor* rowPtr, CColor* colorBake) {
    /*glm::vec2 coords(0.0f, fhpos * s.Scale - s.Offset.Y);
    for (unsigned int c = 0; c < s.Width; c++) {
        coords.x = (c / (float)s.Width - 0.5) * s.Scale - s.Offset.X;
        unsigned int rpix = render2d_mandelbrot_pixel(s.Iterations, coords.x, coords.y);
        rowPtr[c] = colorbake[rpix];
    }*/
    coord2 coords = coord2DEF;
    coords.Y = linePos * FS.Scale - FS.Offset.Y;
    for (unsigned int c = 0; c < FS.Width; c++) {
        coords.X = (c / (float)FS.Width - 0.5) * FS.Scale - FS.Offset.X;
        unsigned int rpix = render2d_mandelbrot_pixel(FS.Iterations, coords.X, coords.Y);
        rowPtr[c] = colorBake[rpix];
    }
}

unsigned int render2d_mandelbrot_pixel(unsigned int maxiter, float x, float y) {
    coord2 pre = coord2DEF;
    coord2 post = coord2DEF;
    unsigned int iter = 0;
    for (;iter < maxiter; iter++) {
        pre.X = (post.X * post.X - post.Y * post.Y) + x;
        pre.Y = (post.Y * post.X + post.X * post.Y) + y;
        if ((pre.X * pre.X + pre.Y * pre.Y) > 1e5f) break;
        post.X = pre.X;
        post.Y = pre.Y;
    }
    return iter-1;
}
