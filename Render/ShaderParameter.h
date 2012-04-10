#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <Base/Base.h>
#include <Render/Shader.h>

class ShaderParameter {
public:
    ShaderParameter(const std::string &parameterName);
    ~ShaderParameter();

    void bind(Shader *shader);

    void setData(GLenum type, unsigned int count, void *data);

private:
    std::string _parameterName;
    GLint _parameterLocation;

    GLenum _type;
    unsigned int _count;
    void *_data;
};

#endif
