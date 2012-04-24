#include <Render/RenderContext.h>

RenderContext::RenderContext(SDL_Window *window) {
    GLenum glewStatus, frameBufferStatus;
    const GLubyte *version;

    // Use SDL to create the RenderContext
    _context = SDL_GL_CreateContext(window);
	
	// Set up vertical sync
	SDL_GL_SetSwapInterval(1);

    version = glGetString(GL_VERSION);
    Info("GL Version: " << version);

    // Setup GLEW
    // Needed for certain functionality to work (glGenVertexArrays, for example)
    glewExperimental = true;

    // Call GLEW setup
    glewStatus = glewInit();
    
    // Check for GLEW errors
    if(glewStatus != GLEW_OK) {
        Error("Error during extension wrangling: " << glewStatus);
        ASSERT(0);
    }
    
    // TODO - This is where we ought to check extensions to make sure all good things are supported
    ASSERT(glewIsSupported("GL_VERSION_3_2"));
    
    // Set up GL state
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(frameBufferStatus) {
        case GL_FRAMEBUFFER_COMPLETE: return;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            Error("Framebuffer status is 'incomplete attachment'");
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            Error("Framebuffer status is 'incomplete dimensions'");
        case GL_FRAMEBUFFER_UNSUPPORTED:
            Error("Framebuffer status is 'unsupported'");
        default:
            Error("Framebuffer status is " << frameBufferStatus);
            ASSERT(0);
            break;
    }
}

RenderContext::~RenderContext() {
    SDL_GL_DeleteContext(_context);
}

void RenderContext::render(const Matrix4 &projection, const Matrix4 &modelView, RenderableList &renderables) {
    //Info("Rendering " << renderables.size() << " renderables");

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
