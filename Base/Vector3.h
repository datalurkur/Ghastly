#ifndef VECTOR3_H
#define VECTOR3_H

#include <Base/Base.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

template <typename T>
class Vector3 {
public:
    union {
        struct { T x, y, z; };
        struct { T w, h, d; };
        struct { T r, g, b; };
    };

public:
    inline Vector3(): x(0), y(0), z(0) {}
    inline Vector3(const Vector3<T> &other): x(other.x), y(other.y), z(other.z) {}
    inline Vector3(const T nX, const T nY, const T nZ): x(nX), y(nY), z(nZ) {}

    inline Vector3<T>& operator=(const Vector3<T> &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    inline Vector3<T> operator+(const Vector3<T> &rhs) const {
        Vector3<T> ret(*this);
        ret += rhs;
        return ret;
    }

    inline Vector3<T> operator-(const Vector3<T> &rhs) const {
        Vector3<T> ret(*this);
        ret -= rhs;
        return ret;
    }

    inline Vector3<T> operator*(const T scalar) const {
        Vector3<T> ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector3<T> operator/(const T scalar) const {
        Vector3<T> ret(*this);
        ret /= scalar;
        return ret;
    }

    inline Vector3<T>& operator+=(const Vector3<T> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    inline Vector3<T>& operator-=(const Vector3<T> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    inline Vector3<T>& operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

   inline Vector3<T>& operator/=(const T scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    inline Vector3<T> operator-() const {
        return Vector3(-x, -y, -z);
    }

    inline bool operator==(const Vector3<T> &rhs) const {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }

    inline bool operator!=(const Vector3<T> &rhs) const {
        return (x != rhs.x || y != rhs.y || z != rhs.z);
    }

    inline T& operator[](const size_t i) {
        ASSERT(i<3);
        return *(&x+i);
    }

    inline T operator[](const size_t i) const {
        ASSERT(i<3);
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

typedef Vector3<float> Vec3f;
typedef Vector3<int> Vec3i;

template <typename T>
T Vector3<T>::length() const {
    return sqrt((x*x) + (y*y) + (z*z));
}

template <typename T>
T Vector3<T>::lengthSquared() const {
    return (x*x) + (y*y) + (z*z);
}

template <typename T>
void Vector3<T>::normalize() {
    T l = length();
    x = x / l;
    y = y / l;
    z = z / l;
}

template <typename T>
std::ostream& operator<<(std::ostream &lhs, const Vector3<T> &rhs) {
    lhs << "Vector3";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    lhs << " " << rhs[2];
    return lhs;
}

template <typename T>
Vector3<T> operator*(const T lhs, const Vector3<T> &rhs) {
    return rhs * lhs;
}

#endif
