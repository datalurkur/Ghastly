#ifndef AABB3_H
#define AABB3_H

#include <Base/Vector3.h>

template <typename T>
class AABB3 {
public:
    AABB3();
    AABB3(const Vector3<T> &v1, const Vector3<T> &v2);
    ~AABB3();

    void setExtents(const Vector3<T> &v1, const Vector3<T> &v2);
    const Vector3<T>& getLowerBound();
    const Vector3<T>& getUpperBound();

    Vector3<T> getCenter() const;

    bool contains(const AABB3<T> &other) const;
    bool overlaps(const AABB3<T> &other) const;

    void expand(const AABB3<T> &other);
    void expand(const Vector3<T> &border);

    inline AABB3<T> operator+(const Vector3<T> &rhs) {
        AABB3<T> ret(*this);
        ret.expand(rhs);
        return ret;
    }

    inline AABB3<T>& operator=(const AABB3<T> &rhs) {
        _lower = rhs._lower;
        _upper = rhs._upper;
        return *this;
    }

    inline bool operator==(const AABB3<T> &rhs) {
        return (_lower == rhs._lower && _upper == rhs._upper);
    }

public:
    static AABB3<T> Combine(const AABB3<T> &b1, const AABB3<T> &b2);

protected:
    Vector3<T> _lower, _upper;
};

template <typename T>
AABB3<T>::AABB3():
_lower(0, 0, 0), _upper(0, 0, 0)
{}

template <typename T>
AABB3<T>::AABB3(const Vector3<T> &v1, const Vector3<T> &v2) {
    setExtents(v1, v2);
}

template <typename T>
AABB3<T>::~AABB3() {}

template <typename T>
void AABB3<T>::setExtents(const Vector3<T> &v1, const Vector3<T> &v2) {
    _lower=Vector3<T>(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.x));
    _upper=Vector3<T>(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

template <typename T>
const Vector3<T>& AABB3<T>::getLowerBound() {
    return _lower;
}

template <typename T>
const Vector3<T>& AABB3<T>::getUpperBound() {
    return _upper;
}

template <typename T>
Vector3<T> AABB3<T>::getCenter() const {
    return (_lower + _upper) * 0.5;
}

template <typename T>
bool AABB3<T>::contains(const AABB3<T> &other) const {
    return (
        (_lower.x <= other._lower.x) &&
        (_lower.y <= other._lower.y) &&
        (_lower.z <= other._lower.z) &&
        (_upper.x >= other._upper.x) &&
        (_upper.y >= other._upper.y) &&
        (_upper.z >= other._upper.z)
    );
}

template <typename T>
bool AABB3<T>::overlaps(const AABB3<T> &other) const {
    Vector3<T> lowerMargin, upperMargin;

    lowerMargin = _lower - other._upper;
    if(lowerMargin.x > 0.0 || lowerMargin.y > 0.0 || lowerMargin.z > 0.0) { return false; }

    upperMargin = other._lower - _upper;
    if(upperMargin.x > 0.0 || upperMargin.y > 0.0 || upperMargin.z > 0.0) { return false; }

    return true;
}

template <typename T>
void AABB3<T>::expand(const AABB3<T> &other) {
    _lower = Vector3<T>(min(_lower.x, other._lower.x), min(_lower.y, other._lower.y), min(_lower.z, other._lower.z));
    _upper = Vector3<T>(max(_upper.x, other._upper.x), max(_upper.y, other._upper.y), max(_upper.z, other._upper.z));
}

template <typename T>
void AABB3<T>::expand(const Vector3<T> &border) {
    _lower -= border;
    _upper += border;
}

template <typename T>
AABB3<T> AABB3<T>::Combine(const AABB3<T> &b1, const AABB3<T> &b2) {
    AABB3<T> ret = b1;
    ret.expand(b2);
    return ret;
}

#endif
