#include <Render/RenderContext.h>
#include <SDL/SDL_opengl.h>
#include <Render/GLHelper.h>

RenderContext::RenderContext(SDL_Window *window) {
    GLenum glewStatus;

    // Use SDL to create the RenderContext
    _context = SDL_GL_CreateContext(window);
    
    // Setup GLEW
    glewStatus = glewInit();
    if(glewStatus != GLEW_OK) {
        Error("Error during extension wrangling: " << glewStatus);
        ASSERT(0);
    }
    
    // TODO - This is where we ought to check extensions to make sure all good things are supported
    
    // Set up GL state
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

RenderContext::~RenderContext() {
    SDL_GL_DeleteContext(_context);
}

void RenderContext::render(const Matrix4 &projection, const Matrix4 &modelView, RenderableList &renderables) {
    //Info("Rendering " << renderables.size() << " renderables");

    // Deprecated by inclusion of the projection and modelview matrices in the shader
    //glMatrixMode(GL_PROJECTION);
    //glLoadMatrixf(projection.ptr());

    //glMatrixMode(GL_MODELVIEW);
    //glLoadMatrixf(modelView.ptr());

    RenderableList::iterator itr = renderables.begin();
    for(; itr != renderables.end(); itr++) {
        (*itr)->render(projection, modelView);
    }
    // FIXME - Add the render queue and group rendering by material
}

void RenderContext::clear() { 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderContext::setViewport(Viewport *viewport) {
    glViewport(viewport->x(), viewport->y(), viewport->w(), viewport->h());
}
