#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include <Engine/Camera.h>

class OrthoCamera: public Camera {
public:
	OrthoCamera(const std::string &name);

	void setup();

	void setAspectRatio(const float ratio);

private:
	float _zoom;
	float _near, _far;
};

#endif