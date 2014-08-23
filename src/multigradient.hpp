#ifndef MULTIGRADIENT_HPP
#define MULTIGRADIENT_HPP

#include "common.hpp"

#include <QColor>

#include <algorithm>
#include <array>
#include <vector>

typedef std::pair<float, QColor> GradientNode;

class MultiGradient {
public:
    MultiGradient();
    MultiGradient(std::initializer_list<GradientNode>);
    ~MultiGradient();
    const GradientNode* Add(float, QColor);
    bool Set(const GradientNode* ptr, float pos);
    bool Set(const GradientNode* ptr, QColor C);
    bool Set(const GradientNode* ptr, float pos, QColor C);
    void Remove(const GradientNode* ptr);
    unsigned int Nodes() const;
    Range GetRange() const;
    QColor Lerp(float) const;
    template <int LEN> std::array<QColor, LEN> Bake() const;
    QColor* Bake(int) const;
    QColor* Bake(float, float, int, int = 0) const;
    const std::vector<GradientNode*>& GetVector() const {return t;}
    //Copy and Assignment
    MultiGradient (const MultiGradient& other);
    MultiGradient& operator=(const MultiGradient& other);
private:
    std::vector<GradientNode*> t;
    struct Compare { bool operator() (GradientNode* A, GradientNode* B) {return A->first < B->first;} } CompareI;
    void Sort();
    void DirtyAdd(float pos, QColor C);
};

#endif // MULTIGRADIENT_HPP
