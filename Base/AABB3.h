#ifndef AABB3_H
#define AABB3_H

#include <Base/Vector3.h>

class AABB3 {
public:
    AABB3();
    AABB3(const Vector3 &v1, const Vector3 &v2);
    ~AABB3();

    void setExtents(const Vector3 &v1, const Vector3 &v2);
    const Vector3& getLowerBound();
    const Vector3& getUpperBound();

    Vector3 getCenter() const;

    bool contains(const AABB3& other) const;
    bool overlaps(const AABB3& other) const;

    void expand(const AABB3& other);
	void expand(const Vector3& border);

	inline AABB3 operator+(const Vector3& rhs) {
		AABB3 ret(*this);
		ret.expand(rhs);
		return ret;
	}

    inline AABB3& operator=(const AABB3& rhs) {
        _lower = rhs._lower;
        _upper = rhs._upper;
        return *this;
    }

	inline bool operator==(const AABB3& rhs) {
		return (_lower == rhs._lower && _upper == rhs._upper);
	}

public:
    static AABB3 Combine(const AABB3& b1, const AABB3& b2);

protected:
    Vector3 _lower, _upper;
};

#endif
