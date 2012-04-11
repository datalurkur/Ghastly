#include <Render/ColorParameter.h>
#include <Base/Assertion.h>

ColorParameter::ColorParameter(const Color4 &color):
    ShaderParameter(false), _color(color)
{
}

const void* ColorParameter::getUniformData() const {
    return _color.ptr();
}
