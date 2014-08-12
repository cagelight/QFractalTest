#include "render_pixel.h"

unsigned int render2d_mandelbrot_pixel(unsigned int maxiter, float x, float y) {
    struct coord2 pre = {.X = 0.0f, .Y = 0.0f};
    struct coord2 post = {.X = 0.0f, .Y = 0.0f};
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
