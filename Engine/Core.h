#ifndef CORE_H
#define CORE_H

#include <Engine/ParentState.h>
#include <Engine/Window.h>
#include <Engine/EventHandler.h>
#include <Engine/WindowListener.h>
#include <Render/Viewport.h>

#define FPS_WINDOW_SIZE 256

class Core: public ParentState, public WindowListener {
public:
    Core();
    ~Core();

    void start();
    void stop();

    void setup();
    void teardown();

    void resizeWindow(const int w, const int h);
    void closeWindow();

    Viewport *getViewport() const;

protected:
    int getTime();

private:
    float trackFPS(int elapsed);

private:
    bool _running;

    EventHandler *_eventHandler;

    Window *_window;
    Viewport *_viewport;
    RenderContext *_renderContext;
	
    int _elapsedSamples[FPS_WINDOW_SIZE];
    int _elapsedIndex;
};

#endif
