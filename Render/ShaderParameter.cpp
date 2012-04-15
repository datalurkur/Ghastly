#include <Render/ShaderParameter.h>
#include <Base/Assertion.h>

ShaderParameter::ShaderParameter(): _hasState(false), _hasUniform(false) {}
ShaderParameter::~ShaderParameter() {}

bool ShaderParameter::hasState() const { return _hasState; }
bool ShaderParameter::hasUniform() const { return _hasUniform; }

void ShaderParameter::enable(Shader *shader) {}
void ShaderParameter::disable(Shader *shader) {}

const void* ShaderParameter::getUniformData() const { return 0; }
