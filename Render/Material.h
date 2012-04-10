#ifndef MATERIAL_H
#define MATERIAL_H

#include <Render/Texture.h>
#include <Render/Shader.h>
#include <Base/Color.h>

class Material {
public:
    Material();
    virtual ~Material();

    void setShader(Shader *shader);
    Shader *getShader();

    void setUniform(const std::string &name, ShaderParameter *param);
    ShaderParameter* getUniform(const std::string &name)

    void enable();
    void disable();

private:
    Shader *_shader;

    typedef std::map<std::string, ShaderParameter*> ShaderParamMap;
    ShaderParamMap _shaderUniforms;
};

#endif
