#ifndef MATRIX4_H
#define MATRIX4_H

#include <Base/Base.h>

class Matrix4 {
protected:
    union {
        float data[4][4];
        float _data[16];
    };

public:
    inline Matrix4() {}

    inline Matrix4(
        float data00, float data01, float data02, float data03,
        float data10, float data11, float data12, float data13,
        float data20, float data21, float data22, float data23,
        float data30, float data31, float data32, float data33
    ) {
        data[0][0] = data00;
        data[0][1] = data01;
        data[0][2] = data02;
        data[0][3] = data03;
        data[1][0] = data10;
        data[1][1] = data11;
        data[1][2] = data12;
        data[1][3] = data13;
        data[2][0] = data20;
        data[2][1] = data21;
        data[2][2] = data22;
        data[2][3] = data23;
        data[3][0] = data30;
        data[3][1] = data31;
        data[3][2] = data32;
        data[3][3] = data33;
    }

    inline explicit Matrix4(float *mat) {
        memcpy(_data, mat, sizeof(float)*16);
    }

    inline Matrix4(const Matrix4 &other) {
        memcpy(_data, other.ptr(), sizeof(float)*16);
    }

    inline Matrix4& operator=(const Matrix4 &other) {
        memcpy(_data, other.ptr(), sizeof(float)*16);
        return *this;
    }

    inline Matrix4 operator*(const Matrix4 &other) const {
        Matrix4 ret;
        ret.data[0][0] = data[0][0] * other.data[0][0] + data[0][1] * other.data[1][0] + data[0][2] * other.data[2][0] + data[0][3] * other.data[3][0];
        ret.data[0][1] = data[0][0] * other.data[0][1] + data[0][1] * other.data[1][1] + data[0][2] * other.data[2][1] + data[0][3] * other.data[3][1];
        ret.data[0][2] = data[0][0] * other.data[0][2] + data[0][1] * other.data[1][2] + data[0][2] * other.data[2][2] + data[0][3] * other.data[3][2];
        ret.data[0][3] = data[0][0] * other.data[0][3] + data[0][1] * other.data[1][3] + data[0][2] * other.data[2][3] + data[0][3] * other.data[3][3];

        ret.data[1][0] = data[1][0] * other.data[0][0] + data[1][1] * other.data[1][0] + data[1][2] * other.data[2][0] + data[1][3] * other.data[3][0];
        ret.data[1][1] = data[1][0] * other.data[0][1] + data[1][1] * other.data[1][1] + data[1][2] * other.data[2][1] + data[1][3] * other.data[3][1];
        ret.data[1][2] = data[1][0] * other.data[0][2] + data[1][1] * other.data[1][2] + data[1][2] * other.data[2][2] + data[1][3] * other.data[3][2];
        ret.data[1][3] = data[1][0] * other.data[0][3] + data[1][1] * other.data[1][3] + data[1][2] * other.data[2][3] + data[1][3] * other.data[3][3];

        ret.data[2][0] = data[2][0] * other.data[0][0] + data[2][1] * other.data[1][0] + data[2][2] * other.data[2][0] + data[2][3] * other.data[3][0];
        ret.data[2][1] = data[2][0] * other.data[0][1] + data[2][1] * other.data[1][1] + data[2][2] * other.data[2][1] + data[2][3] * other.data[3][1];
        ret.data[2][2] = data[2][0] * other.data[0][2] + data[2][1] * other.data[1][2] + data[2][2] * other.data[2][2] + data[2][3] * other.data[3][2];
        ret.data[2][3] = data[2][0] * other.data[0][3] + data[2][1] * other.data[1][3] + data[2][2] * other.data[2][3] + data[2][3] * other.data[3][3];

        ret.data[3][0] = data[3][0] * other.data[0][0] + data[3][1] * other.data[1][0] + data[3][2] * other.data[2][0] + data[3][3] * other.data[3][0];
        ret.data[3][1] = data[3][0] * other.data[0][1] + data[3][1] * other.data[1][1] + data[3][2] * other.data[2][1] + data[3][3] * other.data[3][1];
        ret.data[3][2] = data[3][0] * other.data[0][2] + data[3][1] * other.data[1][2] + data[3][2] * other.data[2][2] + data[3][3] * other.data[3][2];
        ret.data[3][3] = data[3][0] * other.data[0][3] + data[3][1] * other.data[1][3] + data[3][2] * other.data[2][3] + data[3][3] * other.data[3][3];

        return ret;
    }

    inline float* operator[](const size_t i) {
        ASSERT(i<4);
        return data[i];
    }

    inline const float* operator[](const size_t i) const {
        ASSERT(i<4);
        return data[i];
    }

    inline Matrix4 transpose(void) const
    {
        return Matrix4(
            data[0][0], data[1][0], data[2][0], data[3][0],
            data[0][1], data[1][1], data[2][1], data[3][1],
            data[0][2], data[1][2], data[2][2], data[3][2],
            data[0][3], data[1][3], data[2][3], data[3][3]
        );
    }

	inline void translate(const float x, const float y, const float z) {
		Matrix4 result = Matrix4::MakeTranslation(x, y, z);
		(*this) = result * (*this);
	}

    inline float *ptr() {
        return &_data[0];
    }

    inline const float *ptr() const {
        return &_data[0];
    }

public:
    static const Matrix4 Identity;

public:
    static Matrix4 MakeOrtho(float l, float r, float b, float t, float n, float f);
	static Matrix4 MakePerspective(float ratio, float fov, float near, float far);
	static Matrix4 MakeTranslation(float x, float y, float z);
};

std::ostream& operator<<(std::ostream& lhs, const Matrix4 &rhs);

#endif
