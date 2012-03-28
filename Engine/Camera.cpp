#include <Engine/Camera.h>

Camera::Camera(const std::string &name): SceneNode(name) {}

void Camera::setAspectRatio(float ratio) {
	_aspectRatio = ratio;
	recomputeMatrices();
}