#include <Engine/Frustum.h>
#include <Base/Matrix4.h>

Frustum::Frustum(): _projection(Matrix4::Identity), _modelView(Matrix4::Identity) {
}

void Frustum::setProjection(const Matrix4 &matrix) {
	_projection = matrix;
}

const Matrix4 Frustum::getProjection() const {
    return _projection;
}

void Frustum::setModelView(const Matrix4  &matrix) {
	_modelView = matrix;
}

const Matrix4 Frustum::getModelView() const {
	return _modelView;
}

Frustum& Frustum::operator=(const Frustum &other) {
	_projection = other.getProjection();
	return *this;
}

std::ostream& operator<<(std::ostream &lhs, const Frustum &rhs) {
	lhs << "Frustum";
	lhs << " Projection: " << rhs.getProjection();
	return lhs;
}