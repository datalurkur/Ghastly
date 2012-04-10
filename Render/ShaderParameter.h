#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <Base/Base.h>
#include <Render/Shader.h>

class ShaderParameter {
public:
    ShaderParameter(GLenum type, unsigned int count, void *data);
    ~ShaderParameter();

private:
    GLenum _type;
    unsigned int _count;
    void *_data;
};

#endif
