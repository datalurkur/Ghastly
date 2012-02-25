#include <Base/AABB2.h>

AABB2::AABB2():
_lower(0.0, 0.0), _upper(0.0, 0.0)
{}

AABB2::AABB2(const Vector2& v1, const Vector2& v2) {
    setExtents(v1, v2);
}

AABB2::~AABB2() {}

void AABB2::setExtents(const Vector2& v1, const Vector2& v2) {
    _lower=Vector2(min(v1.x, v2.x), min(v1.y, v2.y));
    _upper=Vector2(max(v1.x, v2.x), max(v1.y, v2.y));
}

const Vector2& AABB2::getLowerBound() {
    return _lower;
}

const Vector2& AABB2::getUpperBound() {
    return _upper;
}

Vector2 AABB2::getCenter() const {
    return (_lower + _upper) * 0.5;
}

const float AABB2::getPerimeter() const {
    return ((_upper.x - _lower.x) + (_upper.y - _lower.y)) * 2;
}

bool AABB2::contains(const AABB2& other) const {
    return (
        (_lower.x <= other._lower.x) &&
        (_lower.y <= other._lower.y) &&
        (_upper.x >= other._upper.x) &&
        (_upper.y >= other._upper.y)
    );
}

bool AABB2::overlaps(const AABB2& other) const {
    Vector2 lowerMargin, upperMargin;
    lowerMargin = _lower - other._upper;
    if(lowerMargin.x > 0.0 || lowerMargin.y > 0.0) { return false; }

    upperMargin = other._lower - _upper;
    if(upperMargin.x > 0.0 || upperMargin.y > 0.0) { return false; }

    return true;
}

void AABB2::expand(const AABB2& other) {
    _lower = Vector2(min(_lower.x, other._lower.x), min(_lower.y, other._lower.y));
    _upper = Vector2(max(_upper.x, other._upper.x), max(_upper.y, other._upper.y));
}

void AABB2::expand(const Vector2& border) {
	_lower -= border;
	_upper += border;
}

AABB2 AABB2::Combine(const AABB2& b1, const AABB2& b2) {
    AABB2 ret = b1;
    ret.expand(b2);
    return ret;
}
