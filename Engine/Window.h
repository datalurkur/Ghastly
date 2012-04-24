#ifndef WINDOW_H
#define WINDOW_H

#include <Base/Base.h>

class Window {
public:
    Window(const std::string &name);
    virtual ~Window();

    void setup();
    void teardown();

    void resize(int w, int h);

    void swapBuffers() const;
    
    int getID() const;
    SDL_Window *getSDLWindow();

private:
    std::string _name;
    int _windowFlags;
    SDL_Window *_window;
};

#endif
