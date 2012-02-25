#include <Engine/IsoCamera.h>

IsoCamera::IsoCamera(const std::string &name): Camera(name), _fov(STANDARD_FOV), _near(10.0f), _far(-10.0f) {
	setModelView(Matrix4::MakeTranslation(-2.0f, -2.0f, -3.0f));
}

IsoCamera::~IsoCamera() {
}

void IsoCamera::setup() {
	glEnable(GL_DEPTH_TEST);
}

void IsoCamera::setAspectRatio(const float ratio) {
	setProjection(Matrix4::MakePerspective(ratio, _fov, _near, _far));
}

void IsoCamera::moveRelative(const Vector3 &dir) {
	SceneNode::moveRelative(dir);

	// Dirty the modelview matrix
	// TODO
}

void IsoCamera::zoom(const float amount) {
	moveRelative(Vector3(0.0f, 0.0f, amount));
}

void IsoCamera::scroll(const Vector3 &dir) {
	moveRelative(dir);
}

void IsoCamera::pitch(const float amount) {
}

void IsoCamera::roll(const float amount) {
}
