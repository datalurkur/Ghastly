#include <Engine/OrthoCamera.h>
#include <UI/UIManager.h>
#include <UI/UICursor.h>

UIManager::UIManager(): _camera(0), _cursorEnabled(true) {
    _camera = new OrthoCamera("UIManagerCamera");

    _cursor = new UICursor(Vec2f(0.5f, 0.5f));
    _layers.push_back(_cursor);

    UIElement *root = new UIElement("root", Vec2f(0.5f, 0.5f));
    _layers.push_back(root);
}

UIManager::~UIManager() {
    if(_camera) { delete _camera; }

    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        delete (*itr);
    }
}

void UIManager::toggleCursor(bool enabled) {
    _cursorEnabled = enabled;
}

void UIManager::render(RenderContext *context) {
    LayerList::reverse_iterator layerItr;

    glDisable(GL_DEPTH_TEST);

    // Render the layers from bottom to top
    for(layerItr = _layers.rbegin(); layerItr != _layers.rend(); layerItr++) {
        SceneNode<int>::NodeList visibleNodes;
        RenderableList renderables;

        if(!_cursorEnabled && (*layerItr) == _cursor) { continue; }

        (*layerItr)->getNodes(visibleNodes, _camera);

        SceneNode<int>::NodeList::iterator nodeItr;
        for(nodeItr = visibleNodes.begin(); nodeItr != visibleNodes.end(); nodeItr++) {
            (*nodeItr)->getRenderables(renderables);
        }

        _camera->setup();
        context->render(_camera->getProjection(), _camera->getModelView(), renderables);
    }
    
    glEnable(GL_DEPTH_TEST);
}

void UIManager::update() {
    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        (*itr)->updateCachedValues();
    }
}

void UIManager::onResize(int w, int h) {
    _width = w;
    _height = h;
    _camera->clampEdges(Vec2f(-(w/2.0f),-(h/2.0f)), Vec2f((w/2.0f),(h/2.0f)));

    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        ((UIElement*)(*itr))->resize(w, h);
    }
}

bool UIManager::mouseMoved(MouseMovedEvent *event) {
    // Translate the dimensions into UI space
    Vec2f uiPos;
    uiPos = _cursor->getUIPosition();

    // Add the relative movement and clamp it within the window
    uiPos.x += (event->relativeX() / (float)_width);
    if(uiPos.x < 0.0f) { uiPos.x = 0.0f; }
    else if(uiPos.x > 1.0f) { uiPos.x = 1.0f; }
    
    uiPos.y += (event->relativeY() / (float)_height);
    if(uiPos.y < 0.0f) { uiPos.y = 0.0f; }
    else if(uiPos.y > 1.0f) { uiPos.y = 1.0f; }

    Info("Moving cursor to " << uiPos << " based on relative movement " << Vec2i(event->relativeX(), event->relativeY()));
    // Set the cursor to the new position
    _cursor->setUIPosition(uiPos, _width, _height);

    return true;
}

bool UIManager::mouseButton(MouseButtonEvent *event) {
    return true;
}

int UIManager::getWidth() const { return _width; }
int UIManager::getHeight() const { return _height; }

void UIManager::addElement(UIElement *element) {
    element->resize(_width, _height);
    _layers.back()->addChild(element);
}
