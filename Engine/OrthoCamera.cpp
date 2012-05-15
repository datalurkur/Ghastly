#include <Engine/OrthoCamera.h>

OrthoCamera::OrthoCamera(const std::string &name): Camera(name), _zoom(1.0f), _near(0.0f), _far(1.0f) {}

void OrthoCamera::setup() {
    glDisable(GL_DEPTH_TEST);
}

void OrthoCamera::recomputeMatrices() {
    setProjection(Matrix4::MakeOrtho(-(_aspectRatio/_zoom), (_aspectRatio/_zoom), -(float)1.0f / _zoom, (float)1.0f / _zoom, _near, _far));
}

void OrthoCamera::setZoom(float zoom) {
    _zoom = zoom;
    recomputeMatrices();
}

void OrthoCamera::clampEdges(const Vec2f &lowerLeft, const Vec2f &upperRight) {
    setProjection(Matrix4::MakeOrtho(lowerLeft.x, upperRight.x, lowerLeft.y, upperRight.y, _near, _far));
}
