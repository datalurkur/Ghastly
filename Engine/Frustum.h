#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <Base/Base.h>
#include <Base/Matrix4.h>

class Frustum {
public:
    Frustum();

	void setProjection(const Matrix4 &matrix);
    const Matrix4 getProjection() const;

	void setModelView(const Matrix4 &matrix);
	const Matrix4 getModelView() const;

	Frustum& operator=(const Frustum &other);

private:
	Matrix4 _projection;
	Matrix4 _modelView;
};

std::ostream& operator<<(std::ostream& lhs, const Frustum &rhs);

#endif
