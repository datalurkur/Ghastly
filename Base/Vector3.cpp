#include <Base/Vector3.h>

float Vector3::length() const {
    return sqrt((x*x) + (y*y) + (z*z));
}

float Vector3::lengthSquared() const {
    return (x*x) + (y*y) + (z*z);
}

void Vector3::normalize() {
    float l = length();
    x = x / l;
    y = y / l;
    z = z / l;
}

std::ostream& operator<<(std::ostream &lhs, const Vector3 &rhs) {
    lhs << "Vector3";
    lhs << " " << rhs[0];
    lhs << " " << rhs[1];
    lhs << " " << rhs[2];
    return lhs;
}

Vector3 operator*(const float lhs, const Vector3& rhs) {
	return rhs * lhs;
}
