#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <Render/Shader.h>
#include <Resource/ThreadedResourceManager.h>

class ShaderManager;

class ShaderManager: public ThreadedResourceManager<Shader, ShaderManager> {
protected:
    static void DoLoad(const std::string &name, Shader *shader);
    static const std::string LoadDirectory;

    friend class ThreadedResourceManager<Shader, ShaderManager>;
    friend class ResourceManager<Shader, ShaderManager>;
};

#endif
