#include "common.hpp"
#include "multigradient.hpp"

MultiGradient::MultiGradient(std::initializer_list<GradientNode> l) {
    for (GradientNode v : l) {
        this->Add(v.first, v.second);
    }
}

MultiGradient::MultiGradient() {

}

MultiGradient::~MultiGradient() {
    for(GradientNode* v : t) {
        delete v;
    }
}

const GradientNode* MultiGradient::Add(float pos, QColor C) {
    GradientNode* newNode = new GradientNode(pos, C);
    t.push_back(newNode);
    if (t.size() > 1 && (*(t.end() - 2))->first > pos)
        this->Sort();
    return newNode;
}

bool MultiGradient::Set(const GradientNode* ptr, float pos) {
    for(GradientNode* v : t) {
        if (v == ptr) {
            v->first = pos;
            if ((v-1)->first > v->first || (v+1)->first < v->first)
                this->Sort();
            return true;
        }
    }
    return false;
}

bool MultiGradient::Set(const GradientNode* ptr, QColor C) {
    for(GradientNode* v : t) {
        if (v == ptr) {
            v->second = C;
            return true;
        }
    }
    return false;
}

bool MultiGradient::Set(const GradientNode* ptr, float pos, QColor C) {
    for(GradientNode* v : t) {
        if (v == ptr) {
            v->first = pos;
            v->second = C;
            if ((v-1)->first > v->first || (v+1)->first < v->first)
                this->Sort();
            return true;
        }
    }
    return false;
}

void MultiGradient::Remove(const GradientNode *ptr) {
    for (std::vector<GradientNode*>::iterator i = t.begin(); i != t.end(); i++) {
        if (ptr == *i) {
            GradientNode* optr = *i;
            t.erase(i);
            delete optr;
            return;
        }
    }
}

unsigned int MultiGradient::Nodes() const {
    return t.size();
}

Range MultiGradient::GetRange() const {
    Range S(std::numeric_limits<float>::max(), std::numeric_limits<float>::min());
    for(GradientNode* v : t) {
        if (v->first < S.min)
            S.min = v->first;
        if (v->first > S.max)
            S.max = v->first;
    }
    return S;
}

QColor MultiGradient::Lerp(float pos) const {
    if (this->Nodes() > 0) {
        std::vector<GradientNode*>::const_iterator i = t.begin();
        if (pos <= (*i)->first)
            return (*i)->second;
        while (++i != t.end()) {
            if (pos <= (*i)->first) {
                GradientNode& Left = **(i-1);
                GradientNode& Right = **i;
                qreal dmax = Right.first - Left.first;
                qreal delta = pos - Left.first;
                qreal rpol = delta / dmax;
                qreal lpol = 1.0f - rpol;
                QColor F;
                F.setAlphaF(Left.second.alphaF() * lpol + Right.second.alphaF() * rpol);
                F.setRedF(Left.second.redF() * lpol + Right.second.redF() * rpol);
                F.setGreenF(Left.second.greenF() * lpol + Right.second.greenF() * rpol);
                F.setBlueF(Left.second.blueF() * lpol + Right.second.blueF() * rpol);
                return F;
            }
        }
        return (*(i-1))->second;
    } else {
        return QColor(Qt::black);
    }
}

template <int LEN>
std::array<QColor, LEN> MultiGradient::Bake() const {
    std::array<QColor, LEN> arry;
    Range MM = GetRange();
    float delta = MM.max - MM.min;
    for (int i = 0; i < LEN; i++) {
        float f = (i / (float)LEN) * delta + MM.min;
        arry[i] = this->Lerp(f);
    }
    return arry;
}

QColor* MultiGradient::Bake(int len) const {
    QColor* arry = new QColor[len];
    Range MM = GetRange();
    float delta = MM.max - MM.min;
    for (int i = 0; i < len; i++) {
        float f = (i / (float)(len-1)) * delta + MM.min;
        arry[i] = this->Lerp(f);
    }
    return arry;
}



QColor* MultiGradient::Bake(float min, float max, int len, int overhead) const {
    QColor* arry = new QColor[len];
    float delta = max - min;
    for (int i = -overhead; i < len-overhead; i++) {
        int ia = i + overhead;
        float f = (i / (float)(len-1-(2*overhead))) * delta + min;
        arry[ia] = this->Lerp(f);
    }
    return arry;
}

void MultiGradient::Sort() {
    std::sort(t.begin(), t.end(), CompareI);
}

void MultiGradient::DirtyAdd(float pos, QColor C) {
    t.push_back(new GradientNode(pos, C));
}

MultiGradient::MultiGradient(const MultiGradient& other) {
    for(const GradientNode* v : other.GetVector()) {
        this->DirtyAdd(v->first, v->second);
    }
}

MultiGradient& MultiGradient::operator =(const MultiGradient& other) {
    for(const GradientNode* v : other.GetVector()) {
        this->DirtyAdd(v->first, v->second);
    }
    return *this;
}
