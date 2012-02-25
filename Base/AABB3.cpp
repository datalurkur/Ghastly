#include <Base/AABB3.h>

AABB3::AABB3():
_lower(0.0, 0.0, 0.0), _upper(0.0, 0.0, 0.0)
{}

AABB3::AABB3(const Vector3& v1, const Vector3& v2) {
    setExtents(v1, v2);
}

AABB3::~AABB3() {}

void AABB3::setExtents(const Vector3& v1, const Vector3& v2) {
    _lower=Vector3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.x));
    _upper=Vector3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

const Vector3& AABB3::getLowerBound() {
    return _lower;
}

const Vector3& AABB3::getUpperBound() {
    return _upper;
}

Vector3 AABB3::getCenter() const {
    return (_lower + _upper) * 0.5;
}

bool AABB3::contains(const AABB3& other) const {
    return (
        (_lower.x <= other._lower.x) &&
        (_lower.y <= other._lower.y) &&
        (_lower.z <= other._lower.z) &&
        (_upper.x >= other._upper.x) &&
        (_upper.y >= other._upper.y) &&
        (_upper.z >= other._upper.z)
    );
}

bool AABB3::overlaps(const AABB3& other) const {
    Vector3 lowerMargin, upperMargin;

    lowerMargin = _lower - other._upper;
    if(lowerMargin.x > 0.0 || lowerMargin.y > 0.0 || lowerMargin.z > 0.0) { return false; }

    upperMargin = other._lower - _upper;
    if(upperMargin.x > 0.0 || upperMargin.y > 0.0 || upperMargin.z > 0.0) { return false; }

    return true;
}

void AABB3::expand(const AABB3& other) {
    _lower = Vector3(min(_lower.x, other._lower.x), min(_lower.y, other._lower.y), min(_lower.z, other._lower.z));
    _upper = Vector3(max(_upper.x, other._upper.x), max(_upper.y, other._upper.y), max(_upper.z, other._upper.z));
}

void AABB3::expand(const Vector3& border) {
	_lower -= border;
	_upper += border;
}

AABB3 AABB3::Combine(const AABB3& b1, const AABB3& b2) {
    AABB3 ret = b1;
    ret.expand(b2);
    return ret;
}
