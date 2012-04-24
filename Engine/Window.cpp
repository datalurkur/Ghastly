#include <Engine/Window.h>
#include <Base/Log.h>
#include <Base/Assertion.h>
#include <Base/SDLHelper.h>

Window::Window(const std::string &name): _name(name), _windowFlags(0), _window(0) {
	if(SDL_Init(0) < 0) {
        Error("SDL Failed to initialize");
        ASSERT(0);
    }
	if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		Error("Failed to initialize SDL video subsystem.");
		ASSERT(0);
	}
    setup();
}

Window::~Window() {
	teardown();
	SDL_Quit();
}

void Window::setup() {
    _windowFlags  = 0;
    _windowFlags |= SDL_WINDOW_OPENGL;
    _windowFlags |= SDL_WINDOW_SHOWN;
    _windowFlags |= SDL_WINDOW_RESIZABLE;

    // Set the OpenGL context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
    _window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, _windowFlags);
	if(!_window) {
		Error("Window creation failed");
		ASSERT(0);
	}
	CheckSDLErrors();
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
