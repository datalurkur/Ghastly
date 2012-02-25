#include <Base/Vector2.h>

float Vector2::length() const {
    return sqrt((x*x) + (y*y));
}

float Vector2::lengthSquared() const {
    return (x*x) + (y*y);
}

void Vector2::normalize() {
    float l = length();
    x = x / l;
    y = y / l;
}

int Vector2::arraySize() const {
	return (int)(x*y);
}

std::ostream& operator<<(std::ostream &lhs, const Vector2 &rhs) {
    lhs << "Vector2";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    return lhs;
}

Vector2 operator*(const float lhs, const Vector2& rhs) {
	return rhs * lhs;
}
