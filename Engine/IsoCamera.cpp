#include <Engine/IsoCamera.h>

IsoCamera::IsoCamera(const std::string &name): Camera(name), _fov(STANDARD_FOV), _near(10.0f), _far(-10.0f) {
    setModelView(Matrix4::MakeTranslation(-2.0f, -2.0f, -3.0f));
}

IsoCamera::~IsoCamera() {
}

void IsoCamera::setup() {
    glEnable(GL_DEPTH_TEST);
}

void IsoCamera::recomputeMatrices() {
    setProjection(Matrix4::MakePerspective(_aspectRatio, _fov, _near, _far));
}

void IsoCamera::setZoom(float zoom) {
    Vec3f position = SceneNode::getLocalPosition();
    position.z = zoom;
    SceneNode::setPosition(position);
}

void IsoCamera::moveRelative(const Vec3f &dir) {
    SceneNode::moveRelative(dir);

    // Dirty the modelview matrix
    // TODO
}

void IsoCamera::zoom(float amount) {
    moveRelative(Vec3f(0.0f, 0.0f, amount));
}

void IsoCamera::scroll(const Vec3f &dir) {
    moveRelative(dir);
}

void IsoCamera::pitch(float amount) {
}

void IsoCamera::roll(float amount) {
}
