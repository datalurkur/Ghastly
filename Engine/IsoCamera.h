#ifndef ISOCAMERA_H
#define ISOCAMERA_H

#include <Engine/Camera.h>

#define STANDARD_FOV 3.14159f / 4.0f // 45 degrees

class IsoCamera: public Camera {
public:
	IsoCamera(const std::string &name);
	virtual ~IsoCamera();

	void setup();
	void recomputeMatrices();

	void setZoom(float zoom);

	// Normal Isometric Camera controls
	void moveRelative(const Vector3 &dir);

	void zoom(float amount);
	void scroll(const Vector3 &dir);

	void pitch(float amount);
	void roll(float amount);

private:
	float _fov, _near, _far;
};

#endif