#include <Engine/OrthoCamera.h>
#include <UI/UIManager.h>
#include <UI/UICursor.h>

#include <Resource/MaterialManager.h>

UIManager::UIManager(): _camera(0), _cursorEnabled(true) {
    _camera = new OrthoCamera("UIManagerCamera");

    _cursor = new UICursor(Vec2f(0.5f, 0.5f));
    _layers.push_back(_cursor);

    UIElement *root = new UIElement("root", Vec2f(0.0f, 0.0f));
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
    RenderableList renderables;

    // Render the layers from bottom to top
    for(layerItr = _layers.rbegin(); layerItr != _layers.rend(); layerItr++) {
        SceneNode<int>::NodeList visibleNodes;

        if(!_cursorEnabled && (*layerItr) == _cursor) { continue; }

        (*layerItr)->getNodes(visibleNodes, _camera);

        SceneNode<int>::NodeList::iterator nodeItr;
        for(nodeItr = visibleNodes.begin(); nodeItr != visibleNodes.end(); nodeItr++) {
            (*nodeItr)->getRenderables(renderables);
        }
    }

    _camera->setup();
    context->render(_camera->getProjection(), _camera->getModelView(), renderables);
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
    _camera->clampEdges(Vec2f(0.0,0.0), Vec2f((float)w,(float)h));

    LayerList::iterator itr;
    for(itr = _layers.begin(); itr != _layers.end(); itr++) {
        ((UIElement*)(*itr))->resize(w, h);
    }
}

bool UIManager::mouseMoved(MouseMovedEvent *event) {
    // Translate the dimensions into UI space
    Vec2f uiPos;

    // Add the relative movement and clamp it within the window
    uiPos.x = (event->absoluteX() / (float)_width);
    if(uiPos.x < 0.0f) { uiPos.x = 0.0f; }
    else if(uiPos.x > 1.0f) { uiPos.x = 1.0f; }
    
    uiPos.y = 1.0f - (event->absoluteY() / (float)_height);
    if(uiPos.y < 0.0f) { uiPos.y = 0.0f; }
    else if(uiPos.y > 1.0f) { uiPos.y = 1.0f; }

    // MOVE THE CURSOR
    // Set the cursor to the new position
    _cursor->setUIPosition(uiPos, _width, _height);
    
    // PERFORM HOVER FUNCTIONS
    // Get a list of objects we are now hovering over, as well as a list that have been left
    SceneNode<int>::NodeList hovering, leaving;
    SceneNode<int>::NodeList::iterator nodeItr;
    LayerList::iterator layerItr;

    Vec3i hoverPoint( event->absoluteX()                      , (_height -  event->absoluteY()                      ), 0),
          leavePoint((event->absoluteX() - event->relativeX()), (_height - (event->absoluteY() - event->relativeY())), 0);

    AABB3<int> hoverBounds(hoverPoint),
               leaveBounds(leavePoint);

    for(layerItr = _layers.begin(); layerItr != _layers.end(); layerItr++) {
        if(*layerItr == _cursor) { continue; }
        (*layerItr)->getDifference(hovering, leaving, hoverBounds, leaveBounds);
    }

    for(nodeItr = hovering.begin(); nodeItr != hovering.end(); nodeItr++) {
        ((UIElement*)(*nodeItr))->onHover();
    }

    for(nodeItr = leaving.begin(); nodeItr != leaving.end(); nodeItr++) {
        ((UIElement*)(*nodeItr))->onLeave();
    }

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
