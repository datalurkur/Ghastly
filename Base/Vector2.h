#ifndef VECTOR2_H
#define VECTOR2_H

#include <Base/Base.h>
#include <Base/Log.h>

class Vector2 {
public:
	float x, y;

public:
    inline Vector2(): x(0), y(0) {}
    inline Vector2(const Vector2 &other): x(other.x), y(other.y) {}
	inline Vector2(const float nX, const float nY): x(nX), y(nY) {}

	inline Vector2& operator=(const Vector2 &rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

    inline Vector2 operator+(const Vector2 &rhs) const {
		Vector2 ret(*this);
		ret += rhs;
		return ret;
	}

    inline Vector2 operator-(const Vector2 &rhs) const {
		Vector2 ret(*this);
		ret -= rhs;
		return ret;
	}

    inline Vector2 operator*(const float scalar) const {
        Vector2 ret(*this);
        ret *= scalar;
        return ret;
    }

    inline Vector2 operator/(const float scalar) const {
        Vector2 ret(*this);
        ret /= scalar;
        return ret;
    }

    inline Vector2& operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

    inline Vector2& operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

    inline Vector2& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    inline Vector2& operator/=(const float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
	inline Vector2 operator-() const {
		return Vector2(-x, -y);
	}

	inline bool operator==(const Vector2 &rhs) const {
		return (x == rhs.x && y == rhs.y);
	}

	inline bool operator!=(const Vector2 &rhs) const {
		return (x != rhs.x || y != rhs.y);
	}

    inline float& operator[](const size_t i) {
		ASSERT(i<2);
		return *(&x+i);
	}

    inline float operator[](const size_t i) const {
		ASSERT(i<2);
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

	int arraySize() const;

    void normalize();
};

std::ostream& operator<<(std::ostream& lhs, const Vector2& rhs);
Vector2 operator*(const float lhs, const Vector2& rhs);

#endif
