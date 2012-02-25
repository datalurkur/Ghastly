#include <Engine/OrthoCamera.h>

OrthoCamera::OrthoCamera(const std::string &name): Camera(name), _zoom(0.1f), _near(-10.0f), _far(10.0f) {}

void OrthoCamera::setup() {
	glDisable(GL_DEPTH_TEST);
}

void OrthoCamera::setAspectRatio(const float ratio) {
	setProjection(Matrix4::MakeOrtho(-(ratio/_zoom), (ratio/_zoom), -(float)1.0f / _zoom, (float)1.0f / _zoom, _near, _far));
}