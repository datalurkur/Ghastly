#ifndef VECTOR3_H
#define VECTOR3_H

#include <Base/Base.h>
#include <Base/Log.h>

class Vector3 {
public:
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };

public:
    inline Vector3(): x(0), y(0), z(0) {}
    inline Vector3(const Vector3 &other): x(other.x), y(other.y), z(other.z) {}
	inline Vector3(const float nX, const float nY, const float nZ): x(nX), y(nY), z(nZ) {}

	inline Vector3& operator=(const Vector3 &rhs) {
		x = rhs.x;
		y = rhs.y;
                z = rhs.z;
		return *this;
	}

    inline Vector3 operator+(const Vector3 &rhs) const {
		Vector3 ret(*this);
		ret += rhs;
		return ret;
	}

    inline Vector3 operator-(const Vector3 &rhs) const {
		Vector3 ret(*this);
		ret -= rhs;
		return ret;
	}

    inline Vector3 operator*(const float scalar) const {
        Vector3 ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector3 operator/(const float scalar) const {
        Vector3 ret(*this);
        ret /= scalar;
        return ret;
    }

    inline Vector3& operator+=(const Vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
                z += rhs.z;
		return *this;
	}

    inline Vector3& operator-=(const Vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
                z -= rhs.z;
		return *this;
	}

    inline Vector3& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

   inline Vector3& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline bool operator==(const Vector3 &rhs) const {
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline bool operator!=(const Vector3 &rhs) const {
		return (x != rhs.x || y != rhs.y || z != rhs.z);
	}

    inline float& operator[](const size_t i) {
		ASSERT(i<3);
		return *(&x+i);
	}

    inline float operator[](const size_t i) const {
		ASSERT(i<3);
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

std::ostream& operator<<(std::ostream& lhs, const Vector3& rhs);
Vector3 operator*(const float lhs, const Vector3& rhs);

#endif
