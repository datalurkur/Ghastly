#include <Render/Viewport.h>
#include <Resource/TextureManager.h>
#include <Resource/TTFManager.h>

Viewport::Viewport(): _x(0), _y(0), _w(0), _h(0) {}
Viewport::Viewport(int x, int y, int w, int h): _x(x), _y(y), _w(w), _h(h) {}

void Viewport::resize(int x, int y, int w, int h) {
    Info("Viewport resizing to " << x << "," << y << "," << w << "," << h);
    _x = x;
    _y = y;
    _w = w;
    _h = h;

    ResizeListenerList::iterator itr = _resizeListeners.begin();
    for(; itr != _resizeListeners.end(); itr++) {
        (*itr)->onResize(w, h);
    }

    TextureManager::ReloadAll();
    TTFManager::ReloadAll();
}

void Viewport::registerResizeListener(ResizeListener *listener) {
    ResizeListenerList::iterator itr;
    for(itr = _resizeListeners.begin(); itr != _resizeListeners.end(); itr++) {
        if(*itr == listener) {
            Warn("ResizeListener already registered with viewport");
            return;
        }
    }
    listener->onResize(_w, _h);
    _resizeListeners.push_back(listener);
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
