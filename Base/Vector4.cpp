#include <Base/Vector4.h>

float Vector4::length() const {
    return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

float Vector4::lengthSquared() const {
    return (x*x) + (y*y) + (z*z) + (w*w);
}

void Vector4::normalize() {
    float l = length();
    x = x / l;
    y = y / l;
    z = z / l;
    w = w / l;
}

std::ostream& operator<<(std::ostream &lhs, const Vector4 &rhs) {
    lhs << "Vector4";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    lhs << " " << rhs[2];
    lhs << " " << rhs[3];
    return lhs;
}

Vector4 operator*(const float lhs, const Vector4& rhs) {
	return rhs * lhs;
}
