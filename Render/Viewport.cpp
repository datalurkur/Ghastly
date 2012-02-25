#include <Render/Viewport.h>
#include <Resource/TextureManager.h>

Viewport::Viewport(): _x(0), _y(0), _w(0), _h(0) {}
Viewport::Viewport(int x, int y, int w, int h): _x(x), _y(y), _w(w), _h(h) {}

void Viewport::resize(int x, int y, int w, int h) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;

    CameraMap::iterator itr = _cameras.begin();
    for(; itr != _cameras.end(); itr++) {
        itr->second->setAspectRatio((float)w/h);
    }

	TextureManager::ReloadAll();
}

void Viewport::registerCamera(Camera *camera) {
    CameraMap::iterator itr = _cameras.find(camera->getName());
    if(itr == _cameras.end()) {
        camera->setAspectRatio((float)_w/_h);
        _cameras[camera->getName()] = camera;
    } else {
        Error("Camera " << camera->getName() << " already registered with viewport.");
    }
}

const int Viewport::x() const { return _x; }
const int Viewport::y() const { return _y; }
const int Viewport::w() const { return _w; }
const int Viewport::h() const { return _h; }

std::ostream& operator<<(std::ostream& lhs, const Viewport &rhs) {
	lhs << "Viewport";
	lhs << " x: " << rhs.x();
	lhs << " y: " << rhs.y();
	lhs << " w: " << rhs.w();
	lhs << " h:" << rhs.h();
	return lhs;
}