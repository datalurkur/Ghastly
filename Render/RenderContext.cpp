#include <Render/RenderContext.h>
#include <SDL/SDL_opengl.h>
#include <Render/GLHelper.h>

RenderContext::RenderContext() {
    //glShadeModel(GL_SMOOTH);
    //glDepthFunc(GL_LEQUAL);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //glBlendFunc(GL_ZERO, GL_ONE);
    glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
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
    CheckGLErrors();
}

void RenderContext::clear() { 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::setViewport(Viewport *viewport) {
	glViewport(viewport->x(), viewport->y(), viewport->w(), viewport->h());
}
