#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include <Engine/Camera.h>

class OrthoCamera: public Camera {
public:
	OrthoCamera(const std::string &name);

	void setup();
	void recomputeMatrices();

	void setZoom(float zoom);

private:
	float _zoom;
	float _near, _far;
};

#endif