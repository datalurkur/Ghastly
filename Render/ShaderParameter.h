#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <Base/Base.h>
#include <Render/Shader.h>

class ShaderParameter {
public:
    virtual void enable();
    virtual void disable();

    virtual const void *getUniformData() const = 0;
};

#endif
