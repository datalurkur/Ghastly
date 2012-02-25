#ifndef WINDOW_H
#define WINDOW_H

#include <Base/Base.h>

class Window {
public:
	Window();
	Window(int w, int h);
	virtual ~Window();

	void setup();

	void resize(int w, int h);

	void swapBuffers() const;

private:
	int _w, _h;
	int _videoFlags;
	SDL_Surface *_frameBuffer;
};

#endif
