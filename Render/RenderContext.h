#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <Render/Renderable.h>
#include <Render/Viewport.h>
#include <Base/Matrix4.h>

class RenderContext {
public:
    RenderContext();

	void render(const Matrix4 &projection, const Matrix4 &modelView, RenderableList &renderables);
	void clear();

	void setViewport(Viewport *viewport);
};

#endif