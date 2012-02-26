#ifndef QUADTREESCENEMANAGER_H
#define QUADTREESCENEMANAGER_H

#include <Engine/SceneManager.h>

class QuadTreeSceneManager: public SceneManager {
public:
	QuadTreeSceneManager();
	virtual ~QuadTreeSceneManager();

private:
	int _maxDepth;
};

#endif

