#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include <Engine/Camera.h>

class OrthoCamera: public Camera {
public:
    OrthoCamera(const std::string &name);

    void setup();
    void recomputeMatrices();

    void setZoom(float zoom);
    void clampEdges(const Vec2f &lowerLeft, const Vec2f &upperRight);

private:
    float _zoom;
    float _near, _far;
};

#endif
