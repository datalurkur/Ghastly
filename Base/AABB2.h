#ifndef AABB2_H
#define AABB2_H

#include <Base/Vector2.h>

template <typename T>
class AABB2 {
public:
    AABB2();
    AABB2(const Vector2<T> &v1, const Vector2<T> &v2);
    ~AABB2();

    void setExtents(const Vector2<T> &v1, const Vector2<T> &v2);
    const Vector2<T>& getLowerBound();
    const Vector2<T>& getUpperBound();

    Vector2<T> getCenter() const;
    const float getPerimeter() const;

    bool contains(const AABB2<T>& other) const;
    bool overlaps(const AABB2<T>& other) const;

    void expand(const AABB2<T>& other);
    void expand(const Vector2<T>& border);

    inline AABB2<T> operator+(const Vector2<T>& rhs) {
        AABB2<T> ret(*this);
        ret.expand(rhs);
        return ret;
    }

    inline AABB2<T>& operator=(const AABB2<T>& rhs) {
        _lower = rhs._lower;
        _upper = rhs._upper;
        return *this;
    }
    
    inline bool operator==(const AABB2<T>& rhs) {
        return (_lower == rhs._lower && _upper == rhs._upper);
    }

public:
    static AABB2<T> Combine(const AABB2<T>& b1, const AABB2<T>& b2);

protected:
    Vector2<T> _lower, _upper;
};

template <typename T>
AABB2<T>::AABB2():
_lower(0.0, 0.0), _upper(0.0, 0.0)
{}

template <typename T>
AABB2<T>::AABB2(const Vector2<T>& v1, const Vector2<T>& v2) {
    setExtents(v1, v2);
}

template <typename T>
AABB2<T>::~AABB2() {}

template <typename T>
void AABB2<T>::setExtents(const Vector2<T>& v1, const Vector2<T>& v2) {
    _lower=Vector2<T>(min(v1.x, v2.x), min(v1.y, v2.y));
    _upper=Vector2<T>(max(v1.x, v2.x), max(v1.y, v2.y));
}

template <typename T>
const Vector2<T>& AABB2<T>::getLowerBound() {
    return _lower;
}

template <typename T>
const Vector2<T>& AABB2<T>::getUpperBound() {
    return _upper;
}

template <typename T>
Vector2<T> AABB2<T>::getCenter() const {
    return (_lower + _upper) * 0.5;
}

template <typename T>
const float AABB2<T>::getPerimeter() const {
    return ((_upper.x - _lower.x) + (_upper.y - _lower.y)) * 2;
}

template <typename T>
bool AABB2<T>::contains(const AABB2<T>& other) const {
    return (
        (_lower.x <= other._lower.x) &&
        (_lower.y <= other._lower.y) &&
        (_upper.x >= other._upper.x) &&
        (_upper.y >= other._upper.y)
    );
}

template <typename T>
bool AABB2<T>::overlaps(const AABB2<T>& other) const {
    Vector2<T> lowerMargin, upperMargin;
    lowerMargin = _lower - other._upper;
    if(lowerMargin.x > 0.0 || lowerMargin.y > 0.0) { return false; }

    upperMargin = other._lower - _upper;
    if(upperMargin.x > 0.0 || upperMargin.y > 0.0) { return false; }

    return true;
}

template <typename T>
void AABB2<T>::expand(const AABB2<T>& other) {
    _lower = Vector2<T>(min(_lower.x, other._lower.x), min(_lower.y, other._lower.y));
    _upper = Vector2<T>(max(_upper.x, other._upper.x), max(_upper.y, other._upper.y));
}

template <typename T>
void AABB2<T>::expand(const Vector2<T>& border) {
    _lower -= border;
    _upper += border;
}

template <typename T>
AABB2<T> AABB2<T>::Combine(const AABB2<T>& b1, const AABB2<T>& b2) {
    AABB2<T> ret = b1;
    ret.expand(b2);
    return ret;
}

#endif
