#ifndef CAMERA_H
#define CAMERA_H

#include <Engine/Frustum.h>
#include <Engine/SceneNode.h>
#include <Render/ResizeListener.h>

class Camera: public ResizeListener, public Frustum, public SceneNode<float> {
public:
    Camera(const std::string &name);
    void onResize(int w, int h);

    virtual void setup() = 0;
    virtual void recomputeMatrices() = 0;

    virtual void setZoom(float zoom) = 0;

protected:
    float _aspectRatio;
};

#endif
