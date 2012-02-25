#ifndef WINDOWLISTENER_H
#define WINDOWLISTENER_H

class WindowListener {
public:
	WindowListener();

	virtual void resizeWindow(const int w, const int h) = 0;
	virtual void closeWindow() = 0;
};

#endif