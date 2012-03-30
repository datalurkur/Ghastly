#include <Engine/Camera.h>

Camera::Camera(const std::string &name): SceneNode(name) {}

void Camera::onResize(int w, int h) {
	_aspectRatio = (float)w/h;
	recomputeMatrices();
}
