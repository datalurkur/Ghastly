#ifndef ISOCAMERA_H
#define ISOCAMERA_H

#include <Engine/Camera.h>

#define STANDARD_FOV 3.14159f / 4.0f // 45 degrees

class IsoCamera: public Camera {
public:
	IsoCamera(const std::string &name);
	virtual ~IsoCamera();

	void setup();

	void setAspectRatio(const float ratio);

	// Normal Isometric Camera controls
	void moveRelative(const Vector3 &dir);

	void zoom(const float amount);
	void scroll(const Vector3 &dir);

	void pitch(const float amount);
	void roll(const float amount);

private:
	float _fov, _near, _far;
};

#endif