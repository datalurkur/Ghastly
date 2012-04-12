#include <Base/PropertyMap.h>
#include <Resource/ShaderManager.h>

const std::string ShaderManager::LoadDirectory = "Shader";

void ShaderManager::DoLoad(const std::string &name, Shader *shader) {
	char *fileData;
	unsigned int fileSize;

    PropertyMap *pMap;
    std::list<std::string> keys;
    std::list<std::string>::iterator itr;

    GLuint vShader, gShader, fShader;
    
    bool hasUniformBlock;
    std::string uniformBlockName;

    // Load the PMap
	fileSize = FileSystem::GetFileData(LoadPath() + name, &fileData);
    if(fileSize == 0) {
        Error("Failed to load " << name);
        return;
    }
    pMap = new PropertyMap(fileData);
    free(fileData);
    
    vShader = 0;
    gShader = 0;
    fShader = 0;

    pMap->getKeys(keys);
    for(itr = keys.begin(); itr != keys.end(); itr++) {
        if(*itr == "vertex") {
            std::string vertShaderName;
            pMap->getValue(*itr, vertShaderName);
            fileSize = FileSystem::GetFileData(LoadPath() + (*itr) + "/" +  vertShaderName, &fileData);
            if(fileSize == 0) {
                Error("Vertex shader " << vertShaderName << " failed to load.");
                continue;
            }
            vShader = shader->compile(fileData, GL_VERTEX_SHADER);
        } else if(*itr == "geometry") {
            std::string geoShaderName;
            pMap->getValue(*itr, geoShaderName);
            Error("GLSL Geometry shaders not yet supported.");
            fileSize = FileSystem::GetFileData(LoadPath() + (*itr) + "/" +  geoShaderName, &fileData);
            if(fileSize == 0) {
                Error("Geometry shader " << geoShaderName << " failed to load.");
                continue;
            }
            vShader = shader->compile(fileData, GL_GEOMETRY_SHADER_EXT);
        } else if(*itr == "fragment") {
            std::string fragShaderName;
            pMap->getValue(*itr, fragShaderName);
            fileSize = FileSystem::GetFileData(LoadPath() + (*itr) + "/" +  fragShaderName, &fileData);
            if(fileSize == 0) {
                Error("Fragment shader " << fragShaderName << " failed to load.");
                continue;
            }
            fShader = shader->compile(fileData, GL_FRAGMENT_SHADER);
        } else if(*itr == "uniform_block") {
            pMap->getValue(*itr, uniformBlockName);
            hasUniformBlock = true;
        }
    }
    delete pMap;

    shader->setup(vShader, gShader, fShader);
    
    if(hasUniformBlock) {
        shader->bindToUniformBlock(uniformBlockName);
    }
}
