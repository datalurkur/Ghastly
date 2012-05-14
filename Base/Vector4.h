#ifndef VECTOR4_H
#define VECTOR4_H

#include <Base/Base.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

template <typename T>
class Vector4 {
public:
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };

public:
    inline Vector4(): x(0), y(0), z(0), w(0) {}
    inline explicit Vector4(const T *data): x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
    inline Vector4(const Vector4<T> &other): x(other.x), y(other.y), z(other.z), w(other.w) {}
    inline Vector4(const T nX, const T nY, const T nZ, const T nW): x(nX), y(nY), z(nZ), w(nW) {}

    inline Vector4<T>& operator=(const Vector4<T> &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    inline Vector4<T> operator+(const Vector4<T> &rhs) const {
        Vector4<T> ret(*this);
        ret += rhs;
        return ret;
    }

    inline Vector4<T> operator-(const Vector4<T> &rhs) const {
        Vector4<T> ret(*this);
        ret -= rhs;
        return ret;
    }

    inline Vector4<T> operator*(const T scalar) const {
        Vector4<T> ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector4<T>& operator+=(const Vector4<T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    inline Vector4<T>& operator-=(const Vector4<T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    inline Vector4<T>& operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    inline Vector4<T> operator-() const {
        return Vector4<T>(-x, -y, -z, -w);
    }

    inline bool operator==(const Vector4<T> &rhs) const {
        return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
    }

    inline bool operator!=(const Vector4<T> &rhs) const {
        return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
    }

    inline T& operator[](const size_t i) {
        ASSERT(i<4);
        return *(&x+i);
    }

    inline T operator[](const size_t i) const {
        ASSERT(i<4);
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

    void normalize();
};

typedef Vector4<int> Vec4i;
typedef Vector4<float> Vec4f;

template <typename T>
T Vector4<T>::length() const {
    return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

template <typename T>
T Vector4<T>::lengthSquared() const {
    return (x*x) + (y*y) + (z*z) + (w*w);
}

template <typename T>
void Vector4<T>::normalize() {
    float l = length();
    x = x / l;
    y = y / l;
    z = z / l;
    w = w / l;
}

template <typename T>
std::ostream& operator<<(std::ostream &lhs, const Vector4<T> &rhs) {
    lhs << "Vector4";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    lhs << " " << rhs[2];
    lhs << " " << rhs[3];
    return lhs;
}

template <typename T>
Vector4<T> operator*(const T lhs, const Vector4<T>& rhs) {
    return rhs * lhs;
}

#endif
