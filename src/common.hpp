#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include <stdint.h>

/*struct Color {
    unsigned char A = 0xff, R = 0x00, G = 0x00, B = 0x00;
public:
    Color() : A(0xff), R(0), G(0), B(0) {}
    Color(unsigned int ARGB) : A(((unsigned char*)&ARGB)[0]), R(((unsigned char*)&ARGB)[1]), G(((unsigned char*)&ARGB)[2]), B(((unsigned char*)&ARGB)[3]) {}
    Color(unsigned char A, unsigned char R, unsigned char G, unsigned char B) : A(A), R(R), G(G), B(B) {}
    operator unsigned int() const {
        unsigned int i = 0x00000000;
        i += B;
        i += (G << 8);
        i += (R << 16);
        i += (A << 24);
        return i;
    }
};*/

struct Range {
    float min;
    float max;
    Range(float, float);
};

#endif // GRADIENT_HPP
