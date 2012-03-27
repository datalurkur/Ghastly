#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <Engine/SceneManager.h>
#include <Engine/ResizeListener.h>

class UIManager: public ResizeListener, public SceneManager {
public:
    UIManager();
    virtual ~UIManager();

    void render(RenderContext *context);
	void setAspectRatio(float ratio);

private:
    Camera *_camera;
};

#endif
