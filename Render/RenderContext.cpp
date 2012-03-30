#include <Render/RenderContext.h>
#include <SDL/SDL_opengl.h>

RenderContext::RenderContext() {
    //glShadeModel(GL_SMOOTH);
    //glDepthFunc(GL_LEQUAL);
    
    glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void RenderContext::render(const Matrix4 &projection, const Matrix4 &modelView, RenderableList &renderables) {
	//Info("Rendering " << renderables.size() << " renderables");
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.ptr());

	glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelView.ptr());

	RenderableList::iterator itr = renderables.begin();
	for(; itr != renderables.end(); itr++) {
        (*itr)->render();
	}
	// FIXME - Add the render queue and group rendering by material
    
    // Check for GL Errors
}

void RenderContext::clear() { 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::setViewport(Viewport *viewport) {
	glViewport(viewport->x(), viewport->y(), viewport->w(), viewport->h());
}
