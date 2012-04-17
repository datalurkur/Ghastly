#include <Engine/Window.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

Window::Window(): _w(0), _h(0), _videoFlags(0), _frameBuffer(0), _id(0) { setup(); }
Window::Window(int w, int h): _w(w), _h(h), _videoFlags(0), _frameBuffer(0), _id(0) { setup(); }

Window::~Window() {
    if(_frameBuffer) {
        SDL_FreeSurface(_frameBuffer);
    }
    SDL_Quit();
}

void Window::setup() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error("SDL Failed to initialize");
        ASSERT(0);
    }

    _videoFlags  = 0;
    _videoFlags |= SDL_GL_DOUBLEBUFFER;
    _videoFlags |= SDL_HWSURFACE;
    _videoFlags |= SDL_OPENGL;
    _videoFlags |= SDL_RESIZABLE;
    _videoFlags |= SDL_HWACCEL;
    _videoFlags |= SDL_HWPALETTE;

    resize(_w, _h);
    swapBuffers();
}

void Window::resize(int w, int h) {
    GLenum glewStatus;

    _w = w;
    _h = h;

    if(_frameBuffer) {
        SDL_FreeSurface(_frameBuffer);
    }
    
    // Set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on vertical sync
    // Note: Must be called *before* SDL_SetVideoMode
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

    if(!(_frameBuffer = SDL_SetVideoMode(_w, _h, 32, _videoFlags))) {
        Error("Failed to create frame buffer.");
        ASSERT(0);
    } else {
        Info("Framebuffer setup complete.");
    }
    
    // Setup GLEW
    glewStatus = glewInit();
    if(glewStatus != GLEW_OK) {
        Error("Error during extension wrangling: " << glewStatus);
        ASSERT(0);
    }
    
    // TODO - This is where we ought to check extensions to make sure all good things are supported
}

void Window::swapBuffers() const {
    SDL_GL_SwapBuffers();
}

int Window::getID() const { return _id; }
