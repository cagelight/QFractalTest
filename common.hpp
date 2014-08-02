#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include <algorithm>
#include <array>
#include <vector>

#include <stdint.h>

struct Color {
    unsigned char A = 0xff, R = 0x00, G = 0x00, B = 0x00;
public:
    Color() : A(0xff), R(0), G(0), B(0) {}
    Color(unsigned char A, unsigned char R, unsigned char G, unsigned char B) : A(A), R(R), G(G), B(B) {}
    operator unsigned int() {
        unsigned int i = 0x00000000;
        i += B;
        i += (G << 8);
        i += (R << 16);
        i += (A << 24);
        return i;
    }
};

struct Range {
    float min;
    float max;
    Range(float, float);
};

typedef std::pair<float, Color> GradientNode;

class MultiGradient {
public:
    MultiGradient();
    MultiGradient(std::initializer_list<GradientNode>);
    ~MultiGradient();
    const GradientNode* Add(float, Color);
    bool Set(const GradientNode* ptr, float pos);
    bool Set(const GradientNode* ptr, Color C);
    bool Set(const GradientNode* ptr, float pos, Color C);
    unsigned int Nodes() const;
    Range GetRange() const;
    Color Lerp(float) const;
    template <int LEN> std::array<Color, LEN> Bake() const;
    Color* Bake(int) const;
    Color* Bake(float, float, int, int = 0) const;
    const std::vector<GradientNode*> GetVector() const {return t;}
    //Copy and Assignment
    MultiGradient (const MultiGradient& other);
    MultiGradient& operator=(const MultiGradient& other);
private:
    std::vector<GradientNode*> t;
    struct Compare { bool operator() (GradientNode* A, GradientNode* B) {return A->first < B->first;} } CompareI;
    void Sort();
    void DirtyAdd(float pos, Color C);
};

#endif // GRADIENT_HPP
