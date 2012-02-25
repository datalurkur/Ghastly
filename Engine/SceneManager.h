#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <Engine/SceneNode.h>

class Camera;
class RenderContext;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

	template <typename T>
	void addNode(T *node);

	template <typename T>
	T* getNode(const std::string &name) const;

	template <typename T>
	void deleteNode(const std::string &name);

    void update();
	void render(Camera *camera, RenderContext *context);

private:
	SceneNode *_root;
	NodeMap _nodes;
};

template <typename T>
void SceneManager::addNode(T *node) {
	Info("Adding " << node->getName() << " to scene");
	_root->addChild(node);
}

template <typename T>
T* SceneManager::getNode(const std::string &name) const {
	NodeMap::const_iterator itr = _nodes.find(name);
	if(itr != _nodes.end() && itr->second->getType() == T::NodeType) {
		return static_cast<T*>(itr->second);
	} else {
		return 0;
	}
}

template <typename T>
void SceneManager::deleteNode(const std::string &name) {
	NodeMap::iterator itr = _nodes.find(name);
	if(itr != _nodes.end() && itr->second->getType() == T::NodeType) {
		_nodes.erase(itr);
		delete itr->second;
	}
}

#endif
