#ifndef COLORPARAMETER_H
#define COLORPARAMETER_H

#include <Render/ShaderParameter.h>
#include <Base/Color.h>

class ColorParameter: public ShaderParameter {
public:
    ColorParameter(const Color4 &color);
    const void* getUniformData() const;

private:
    Color4 _color;
};

#endif
