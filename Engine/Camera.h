#ifndef CAMERA_H
#define CAMERA_H

#include <Engine/Frustum.h>
#include <Engine/SceneNode.h>
#include <Engine/ResizeListener.h>

class Camera: public ResizeListener, public Frustum, public SceneNode {
public:
    Camera(const std::string &name);
	virtual void setup() = 0;
};

#endif
