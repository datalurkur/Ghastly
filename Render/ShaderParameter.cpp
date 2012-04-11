#include <Render/ShaderParameter.h>
#include <Base/Assertion.h>

ShaderParameter::ShaderParameter(bool hasState): _hasState(hasState) {}

bool ShaderParameter::hasState() const { return _hasState; }

void ShaderParameter::enable() {}
void ShaderParameter::disable() {}
