#include <Base/Base.h>
#include <Base/PropertyMap.h>
#include <Render/TextureParameter.h>
#include <Render/ColorParameter.h>
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
    
    // The shader must be set first
    std::string shaderName;
    if(pMap->hasKey("shader")) {
        pMap->getValue("shader", shaderName);
    } else {
        shaderName = "default";
    }
    material->setShader(ShaderManager::Get(shaderName));

    pMap->getKeys(keys);
    for(itr = keys.begin(); itr != keys.end(); itr++) {
        bool blockParam;
        std::string type, value, block, uniform;
        
        PropertyMap::SeparateTypedValue(pMap->getValue(*itr), type, value);
        blockParam = PropertyMap::ExtractScope(*itr, block, uniform);

        if(type == "color4") {
            Color4 color;
            PropertyMap::ExtractValue(value, color);
            if(blockParam) {
                material->setParameter(new ColorParameter(block, uniform, color));
            } else {
                material->setParameter(new ColorParameter(*itr, color));
            }
        } else if(type == "texture") {
            std::string textureName;
            Texture *texture;
            PropertyMap::ExtractValue(value, textureName);
            texture = TextureManager::Get(textureName);
            
            // Textures can't be stored in UBOs
            material->setParameter(new TextureParameter(*itr, texture));
        }
    }
    
    delete pMap;
}
