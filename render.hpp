#ifndef RENDER_HPP
#define RENDER_HPP

#include <QImage>

#include "fractal.hpp"

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

namespace render {
    void initialize();
    QImage get_scaled_copy(int, int);
    QImage get_image_copy();
    QImage render_preview(fractal::Settings);
    void render(fractal::Settings);
    RenderProgress get_progress();
    void terminate();
}

#endif // RENDER_HPP
