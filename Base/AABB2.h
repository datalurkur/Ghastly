#ifndef AABB2_H
#define AABB2_H

#include <Base/Vector2.h>

class AABB2 {
public:
    AABB2();
    AABB2(const Vector2 &v1, const Vector2 &v2);
    ~AABB2();

    void setExtents(const Vector2 &v1, const Vector2 &v2);
    const Vector2& getLowerBound();
    const Vector2& getUpperBound();

    Vector2 getCenter() const;
    const float getPerimeter() const;

    bool contains(const AABB2& other) const;
    bool overlaps(const AABB2& other) const;

    void expand(const AABB2& other);
	void expand(const Vector2& border);

	inline AABB2 operator+(const Vector2& rhs) {
		AABB2 ret(*this);
		ret.expand(rhs);
		return ret;
	}

    inline AABB2& operator=(const AABB2& rhs) {
        _lower = rhs._lower;
        _upper = rhs._upper;
        return *this;
    }
    
	inline bool operator==(const AABB2& rhs) {
		return (_lower == rhs._lower && _upper == rhs._upper);
	}

public:
    static AABB2 Combine(const AABB2& b1, const AABB2& b2);

protected:
    Vector2 _lower, _upper;
};

#endif
