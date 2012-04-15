#include <Render/ColorParameter.h>
#include <Base/Assertion.h>

ColorParameter::ColorParameter(const Color4 &color): _color(color) {
    // TODO - Add a check to see if UBOs are supported, fall back to setting uniforms manually
    _hasUniform = true;
}

const void* ColorParameter::getUniformData() const {
    return _color.ptr();
}
