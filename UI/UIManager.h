#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <Engine/SceneManager.h>

class UIManager: public SceneManager {
public:
    UIManager();
    virtual ~UIManager();

    void render(RenderContext *context);

private:
    Camera *_camera;
};

#endif
