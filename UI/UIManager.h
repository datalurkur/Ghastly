#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <UI/UIElement.h>
#include <Engine/MouseListener.h>
#include <Engine/SceneNode.h>
#include <Engine/OrthoCamera.h>
#include <Render/ResizeListener.h>
#include <Render/RenderContext.h>

class UICursor;

class UIManager: public ResizeListener, public MouseListener {
public:
    UIManager();
    virtual ~UIManager();

    void toggleCursor(bool enabled);

    void render(RenderContext *context);
    void update();

    void onResize(int w, int h);
    bool mouseMoved(MouseMovedEvent *event);
    bool mouseButton(MouseButtonEvent *event);

    int getWidth() const;
    int getHeight() const;

    void addElement(UIElement *element);

private:
    int _width, _height;
    OrthoCamera *_camera;

    typedef std::list<SceneNode<int>*> LayerList;
    LayerList _layers;

    UICursor *_cursor;
    bool _cursorEnabled;
};

#endif
