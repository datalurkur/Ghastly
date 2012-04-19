#ifndef MATERIAL_H
#define MATERIAL_H

#include <Render/Texture.h>
#include <Render/Shader.h>
#include <Render/ShaderParameter.h>
#include <Base/Color.h>

class Material {
public:
    Material();
    virtual ~Material();
    
    void teardown();
    
    void setShader(Shader *shader);
    Shader *getShader();

    void setParameter(ShaderParameter *param);

    void enable();
    void disable();

private:
    Shader *_shader;
    
    typedef std::map<std::string,UniformBuffer*> UniformBufferMap;
    UniformBufferMap _ubos;

    typedef std::map<std::string,ShaderParameter*> ShaderParamMap;
    ShaderParamMap _shaderParams;
    
    friend class MaterialManager;
};

#endif
