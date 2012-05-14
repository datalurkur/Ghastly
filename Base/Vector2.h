#ifndef VECTOR2_H
#define VECTOR2_H

#include <Base/Base.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

template <typename T>
class Vector2 {
public:
    union {
        struct { T x, y; };
        struct { T w, h; };
    };

public:
    inline Vector2(): x(0), y(0) {}
    inline Vector2(const Vector2<T> &other): x(other.x), y(other.y) {}
    inline Vector2(const T nX, const T nY): x(nX), y(nY) {}

    inline Vector2& operator=(const Vector2<T> &rhs) {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    inline Vector2 operator+(const Vector2<T> &rhs) const {
        Vector2<T> ret(*this);
        ret += rhs;
        return ret;
    }

    inline Vector2 operator-(const Vector2<T> &rhs) const {
        Vector2<T> ret(*this);
        ret -= rhs;
        return ret;
    }

    inline Vector2<T> operator*(const T scalar) const {
        Vector2<T> ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector2<T> operator/(const T scalar) const {
        Vector2<T> ret(*this);
        ret /= scalar;
        return ret;
    }

    inline Vector2<T>& operator+=(const Vector2<T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    inline Vector2<T>& operator-=(const Vector2<T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    inline Vector2<T>& operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    inline Vector2<T>& operator/=(const T scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    inline Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }

    inline bool operator==(const Vector2<T> &rhs) const {
        return (x == rhs.x && y == rhs.y);
    }

    inline bool operator!=(const Vector2<T> &rhs) const {
        return (x != rhs.x || y != rhs.y);
    }

    inline T& operator[](const size_t i) {
        ASSERT(i<2);
        return *(&x+i);
    }

    inline T operator[](const size_t i) const {
        ASSERT(i<2);
        return *(&x+i);
    }

    inline T *ptr() {
        return &x;
    }

    inline const T *ptr() const {
        return &x;
    }

    T length() const;
    T lengthSquared() const;

    int arraySize() const;

    void normalize();
};

typedef Vector2<int> Vec2i;
typedef Vector2<float> Vec2f;

template <typename T>
T Vector2<T>::length() const {
    return sqrt((x*x) + (y*y));
}

template <typename T>
T Vector2<T>::lengthSquared() const {
    return (x*x) + (y*y);
}

template <typename T>
void Vector2<T>::normalize() {
    T l = length();
    x = x / l;
    y = y / l;
}

template <typename T>
int Vector2<T>::arraySize() const {
    return (int)(x*y);
}

template <typename T>
std::ostream& operator<<(std::ostream &lhs, const Vector2<T> &rhs) {
    lhs << "Vector2";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    return lhs;
}

template <typename T>
Vector2<T> operator*(const T lhs, const Vector2<T> &rhs) {
    return rhs * lhs;
}

#endif
