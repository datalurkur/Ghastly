#include <Engine/SceneManager.h>
#include <Engine/Camera.h>
#include <Render/RenderContext.h>

SceneManager::SceneManager() {
	_root = new SceneNode("root");
}

SceneManager::~SceneManager() {
	delete _root;
}

void SceneManager::render(Camera *camera, RenderContext *context) {
	NodeList visibleNodes;
	RenderableList renderables;

	_root->getNodes(visibleNodes, camera);

//	Info("Rendering " << visibleNodes.size() << " visible nodes");
	NodeList::iterator itr = visibleNodes.begin();
	for(; itr != visibleNodes.end(); itr++) {
//		Info("Rendering " << (*itr)->getName());
		(*itr)->getRenderables(renderables);
	}

	camera->setup();
	context->render(camera->getProjection(), camera->getModelView(), renderables);
}

void SceneManager::update() {
    _root->updateCachedValues();
}