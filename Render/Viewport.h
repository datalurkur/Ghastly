#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <Base/Base.h>
#include <Engine/ResizeListener.h>

class Viewport {
public:
    Viewport();
    Viewport(int nX, int nY, int nW, int nH);

    void resize(int nX, int nY, int nW, int nH);
    void registerResizeListener(ResizeListener *listener);

    const int x() const;
    const int y() const;
    const int w() const;
    const int h() const;

private:
	typedef std::list<ResizeListener*> ResizeListenerList;

    int _x, _y, _w, _h;
    ResizeListenerList _resizeListeners;
};

std::ostream& operator<<(std::ostream& lhs, const Viewport &rhs);

#endif
