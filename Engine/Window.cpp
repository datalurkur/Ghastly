#include <Engine/Window.h>
#include <Base/Log.h>

Window::Window(): _w(0), _h(0), _videoFlags(0), _frameBuffer(0) { setup(); }
Window::Window(int w, int h): _w(w), _h(h), _videoFlags(0), _frameBuffer(0) { setup(); }

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
	_w = w;
	_h = h;

	if(_frameBuffer) {
		SDL_FreeSurface(_frameBuffer);
	}

	if(!(_frameBuffer = SDL_SetVideoMode(_w, _h, 32, _videoFlags))) {
		Error("Failed to create frame buffer.");
        ASSERT(0);
	}
}

void Window::swapBuffers() const {
	SDL_GL_SwapBuffers();
}