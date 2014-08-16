#ifndef RENDER_HPP
#define RENDER_HPP

#include <QString>
#include <QImage>

#include "fractal_meta.hpp"

class RenderProgress {
public:
    void setMax(unsigned long nmax) {max = nmax;}
    void update(long progressMade) {last = progressMade; cur += last;}
    float totalProgress() const {return cur / (float)max;}
    unsigned long getMax() const {return max;}
    unsigned long getCur() const {return cur;}
    long getLast() const {return last;}
    void reset() {cur = 0; last = 0;}
private:
    unsigned long max = 0;
    unsigned long cur = 0;
    long last = 0;

};

/*
class Bakemap {
public:
    const unsigned int width, height;
    const unsigned short maxiter;
    Bakemap(unsigned int width, unsigned int height, unsigned short maxiter) : width(width), height(height), maxiter(maxiter) {
        data = new unsigned short*[height];
        for (unsigned int h = 0; h < height; h++) {
            data[h] = new unsigned short[width];
            for (unsigned int w = 0; w < width; w++) {
                data[h][w] = 1;
            }
        }
    }
    ~Bakemap() {
        for (unsigned int h = 0; h < height; h++) {
            delete[] data[h];
        }
        delete[] data;
    }
    unsigned short* getRow(int row) {
        return data[row];
    }
    QImage createImage(FractPostSettings FPS) {
        Color* bakearray = FPS.Gradient.Bake(maxiter);
        QImage img(width, height, QImage::Format_ARGB32);
        for (unsigned int h = 0; h < height; h++) {
            unsigned int* rowPtr = (unsigned int*)img.scanLine(h);
            for (unsigned int w = 0; w < width; w++) {
                unsigned short b = data[h][w];
                rowPtr[w] = bakearray[b];
            }
        }
        delete[] bakearray;
        return img;
    }

private:
    unsigned short **data;
};
*/

extern "C" {
    #include "render_pixel.h"
}

namespace render {
    void initialize();
    QImage get_scaled_copy(int, int);
    QImage get_image_copy();
    void save_render(const QString&);
    void render(Fract);
    void stop_render();
    RenderProgress get_progress();
    void terminate();
}

#endif // RENDER_HPP
