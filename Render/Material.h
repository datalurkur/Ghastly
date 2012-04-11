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

    void setShader(Shader *shader);
    Shader *getShader();

    void setUniform(const std::string &name, const ShaderParameter &param);

    void enable();
    void disable();

private:
    Shader *_shader;
    UniformBuffer *_ubo;
};

#endif
