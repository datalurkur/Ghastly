#include <Engine/Window.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

Window::Window(const std::string &name, int w, int h): _name(name), _windowFlags(0), _window(0) {
    setup();
    resize(w, h);
}

Window::~Window() { teardown(); }

void Window::setup() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error("SDL Failed to initialize");
        ASSERT(0);
    }

    _windowFlags  = 0;
    _windowFlags |= SDL_WINDOW_OPENGL;
    _windowFlags |= SDL_WINDOW_SHOWN;

    // Set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
}

void Window::teardown() {
    if(_window) {
        SDL_DestroyWindow(_window);
        _window = 0;
    }
}

void Window::resize(int w, int h) {
    // Destroy the old window
    teardown();
    
    // Create the new window
    _window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, _windowFlags);
}

void Window::swapBuffers() const {
    SDL_GL_SwapWindow(_window);
}

int Window::getID() const {
    return SDL_GetWindowID(_window);
}

SDL_Window *Window::getSDLWindow() {
    return _window;
}