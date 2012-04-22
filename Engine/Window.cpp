#include <Engine/Window.h>
#include <Base/Log.h>
#include <Base/Assertion.h>
#include <Base/SDLHelper.h>

Window::Window(const std::string &name, int w, int h): _name(name), _windowFlags(0), _window(0) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        Error("SDL Failed to initialize");
        ASSERT(0);
    }
    setup();
    resize(w, h);
}

Window::~Window() {
	teardown();
	SDL_Quit();
}

void Window::setup() {    
    _windowFlags  = 0;
    _windowFlags |= SDL_WINDOW_OPENGL;
    _windowFlags |= SDL_WINDOW_SHOWN;

    // Set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    _window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, _windowFlags);
}

void Window::teardown() {
    if(_window) {
        SDL_DestroyWindow(_window);
        _window = 0;
    }
}

void Window::resize(int w, int h) {
	SDL_SetWindowSize(_window, w, h);
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
