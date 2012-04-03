#include <UI/UIManager.h>
#include <Engine/OrthoCamera.h>

UIManager::UIManager(): _camera(0) {
    _camera = new OrthoCamera("UIManagerCamera");
    _rootContainer = new UIElement("root", Vector2(0.5f, 0.5f));
    addNode(_rootContainer);
}

UIManager::~UIManager() {
    if(_camera) { delete _camera; }
}

void UIManager::render(RenderContext *context) {
    SceneManager::render(_camera, context);
}

void UIManager::onResize(int w, int h) {
    _width = w;
    _height = h;
	_camera->clampEdges(Vector2(-(w/2.0f),-(h/2.0f)), Vector2((w/2.0f),(h/2.0f)));

    _rootContainer->resize(w, h);
}

int UIManager::getWidth() const { return _width; }
int UIManager::getHeight() const { return _height; }

void UIManager::addElement(UIElement *element) {
    element->resize(_width, _height);
    _rootContainer->addChild(element);
}