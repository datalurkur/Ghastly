#include <Engine/Core.h>
#include <Base/Log.h>

Core::Core(): _running(false), _renderContext(0), _elapsedIndex(0) {
    setup();
}

Core::~Core() {
    teardown();
}

void Core::setup() {
	// Set up FPS sampling
	for(int i = 0; i < FPS_WINDOW_SIZE; i++) {
		_elapsedSamples[i] = 0;
	}
	
    _core = this;
    
    Log::EnableAllChannels();

	_window = new Window();
    _viewport = new Viewport();
    
	_eventHandler = new EventHandler();
	_eventHandler->addWindowListener(this);
    _eventHandler->addKeyboardListener(this);

    // FIXME - Set this with an options class
    resizeWindow(640, 480);
}

void Core::teardown() {
    ParentState::teardown();

    if(_eventHandler) {
        delete _eventHandler;
        _eventHandler = 0;
    }
    if(_renderContext) {
        delete _renderContext;
        _renderContext = 0;
    }
    if(_viewport) {
        delete _viewport;
        _viewport = 0;
    }
    if(_window) {
        delete _window;
        _window = 0;
    }
}

void Core::start() {
    int lastTime, elapsedTime;

    lastTime = getTime();
    _running = true;

    Info("Main loop starting.");
    while(_running) {
        int currentTime = getTime();
        elapsedTime = currentTime - lastTime;
		
		//Info("FPS: " << trackFPS(elapsedTime) << "(" << elapsedTime << ")");

        _eventHandler->handleEvents();

        update(elapsedTime);
		_renderContext->clear();
        render(_renderContext);
        _window->swapBuffers();

        lastTime = currentTime;
    }

    // Cleanup
    Info("Main loop halted, exiting.");
}

void Core::stop() {
    _running = false;
}

void Core::resizeWindow(const int w, const int h) {
    if(_renderContext) { delete _renderContext; }

    _window->resize(w, h);
    _viewport->resize(0, 0, w, h);
    
    _renderContext = new RenderContext();
    _renderContext->setViewport(_viewport);
}

void Core::closeWindow() {
	stop();
    flushStates();
}

int Core::getTime() {
    return SDL_GetTicks();
}

Viewport* Core::getViewport() const {
    return _viewport;
}

float Core::trackFPS(int elapsed) {
	int sum;
	int i;
	
	_elapsedSamples[_elapsedIndex] = elapsed;
	_elapsedIndex++;
	if(_elapsedIndex > FPS_WINDOW_SIZE) {
		_elapsedIndex = 0;
	}
	
	sum = 0;
	for(i = 0; i < FPS_WINDOW_SIZE; i++) {
		sum += _elapsedSamples[i];
	}

	return (1000 * FPS_WINDOW_SIZE) / (float)sum;
}
