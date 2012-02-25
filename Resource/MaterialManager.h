#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <Render/Material.h>
#include <Resource/ResourceManager.h>

class MaterialManager;

class MaterialManager: public ResourceManager<Material, MaterialManager> {
protected:
    static void DoLoad(const std::string &name, Material *material);
    static const std::string LoadDirectory;
	friend class ResourceManager<Material, MaterialManager>;
};

#endif
