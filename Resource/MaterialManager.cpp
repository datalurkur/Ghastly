#include <Base/Base.h>
#include <Base/PropertyMap.h>
#include <Resource/MaterialManager.h>
#include <Resource/TextureManager.h>
#include <Resource/ShaderManager.h>

const std::string MaterialManager::LoadDirectory = "Material";

void MaterialManager::DoLoad(const std::string &name, Material *material) {
	char *fileData;
	unsigned int fileSize;

    PropertyMap *pMap;
    std::list<std::string> keys;
    std::list<std::string>::iterator itr;

    // Load the PMap
	fileSize = FileSystem::GetFileData(LoadPath() + name, &fileData);
    if(fileSize == 0) {
        Error("Failed to load " << name);
        return;
    }
    pMap = new PropertyMap(fileData);
    free(fileData);
    
    pMap->getKeys(keys);
    for(itr = keys.begin(); itr != keys.end(); itr++) {
        if(*itr == "color4") {
            Color4 color;
            pMap->getValue(*itr, color);
            material->setColor(color);
        } else if(*itr == "texture") {
            std::string textureName;
            Texture *texture;
            pMap->getValue(*itr, textureName);
            texture = TextureManager::Get(textureName);
            material->setTexture(texture);
        } else if(*itr == "shader") {
            std::string shaderName;
            Shader *shader;
            pMap->getValue(*itr, shaderName);
            shader = ShaderManager::Get(shaderName);
            material->setShader(shader);
        }
    }
    
    delete pMap;
}
