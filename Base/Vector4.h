#ifndef VECTOR4_H
#define VECTOR4_H

#include <Base/Base.h>
#include <Base/Log.h>

class Vector4 {
public:
    union {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
    };

public:
    inline Vector4(): x(0), y(0), z(0), w(0) {}
    inline Vector4(const Vector4 &other): x(other.x), y(other.y), z(other.z), w(other.w) {}
	inline Vector4(const float nX, const float nY, const float nZ, const float nW): x(nX), y(nY), z(nZ), w(nW) {}

	inline Vector4& operator=(const Vector4 &rhs) {
		x = rhs.x;
		y = rhs.y;
        z = rhs.z;
        w = rhs.w;
		return *this;
	}

    inline Vector4 operator+(const Vector4 &rhs) const {
		Vector4 ret(*this);
		ret += rhs;
		return ret;
	}

    inline Vector4 operator-(const Vector4 &rhs) const {
		Vector4 ret(*this);
		ret -= rhs;
		return ret;
	}

    inline Vector4 operator*(const float scalar) const {
        Vector4 ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector4& operator+=(const Vector4 &rhs) {
		x += rhs.x;
		y += rhs.y;
        z += rhs.z;
        w += rhs.w;
		return *this;
	}

    inline Vector4& operator-=(const Vector4 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
		return *this;
	}

    inline Vector4& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

	inline Vector4 operator-() const {
		return Vector4(-x, -y, -z, -w);
	}

	inline bool operator==(const Vector4 &rhs) const {
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	inline bool operator!=(const Vector4 &rhs) const {
		return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
	}

    inline float& operator[](const size_t i) {
		ASSERT(i<4);
		return *(&x+i);
	}

    inline float operator[](const size_t i) const {
		ASSERT(i<4);
		return *(&x+i);
	}

	inline float *ptr() {
		return &x;
	}

	inline const float *ptr() const {
		return &x;
	}

    float length() const;
    float lengthSquared() const;

    void normalize();
};

std::ostream& operator<<(std::ostream& lhs, const Vector4& rhs);
Vector4 operator*(const float lhs, const Vector4& rhs);

#endif
