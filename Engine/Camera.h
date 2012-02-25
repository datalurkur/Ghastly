#ifndef CAMERA_H
#define CAMERA_H

#include <Engine/Frustum.h>
#include <Engine/SceneNode.h>

class Camera: public Frustum, public SceneNode {
public:
    Camera(const std::string &name);

	virtual void setup() = 0;

	virtual void setAspectRatio(const float ratio) = 0;

private:
};

#endif
