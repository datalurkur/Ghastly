#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <Base/Base.h>
#include <Engine/Camera.h>

class Viewport {
public:
    Viewport();
    Viewport(int nX, int nY, int nW, int nH);

    void resize(int nX, int nY, int nW, int nH);
    void registerCamera(Camera *camera);

    const int x() const;
    const int y() const;
    const int w() const;
    const int h() const;

private:
    typedef std::map<std::string, Camera*> CameraMap;

    int _x, _y, _w, _h;
    CameraMap _cameras;
};

std::ostream& operator<<(std::ostream& lhs, const Viewport &rhs);

#endif
