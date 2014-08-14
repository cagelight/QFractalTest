#ifndef MULTIGRADIENT_HPP
#define MULTIGRADIENT_HPP

#include "common.hpp"
#include "fractal_settings.h"

#include <algorithm>
#include <array>
#include <vector>

typedef std::pair<float, CColor> GradientNode;

class MultiGradient {
public:
    MultiGradient();
    MultiGradient(std::initializer_list<GradientNode>);
    ~MultiGradient();
    const GradientNode* Add(float, CColor);
    bool Set(const GradientNode* ptr, float pos);
    bool Set(const GradientNode* ptr, CColor C);
    bool Set(const GradientNode* ptr, float pos, CColor C);
    void Remove(const GradientNode* ptr);
    unsigned int Nodes() const;
    Range GetRange() const;
    CColor Lerp(float) const;
    template <int LEN> std::array<CColor, LEN> Bake() const;
    CColor* Bake(int) const;
    CColor* Bake(float, float, int, int = 0) const;
    const std::vector<GradientNode*>& GetVector() const {return t;}
    //Copy and Assignment
    MultiGradient (const MultiGradient& other);
    MultiGradient& operator=(const MultiGradient& other);
private:
    std::vector<GradientNode*> t;
    struct Compare { bool operator() (GradientNode* A, GradientNode* B) {return A->first < B->first;} } CompareI;
    void Sort();
    void DirtyAdd(float pos, CColor C);
};

#endif // MULTIGRADIENT_HPP
