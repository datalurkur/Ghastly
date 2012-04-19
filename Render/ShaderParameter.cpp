#include <Render/ShaderParameter.h>
#include <Base/Assertion.h>

ShaderParameter::ShaderParameter(bool hasState):
    _hasState(hasState), _hasUniform(false), _blockUniform(false) {}
ShaderParameter::ShaderParameter(const std::string &uniformName, bool hasState):
    _hasState(hasState), _hasUniform(true), _uniformName(uniformName), _blockUniform(false) {}
ShaderParameter::ShaderParameter(const std::string &blockName, const std::string &uniformName, bool hasState):
    _hasState(hasState), _hasUniform(true), _uniformName(uniformName), _blockUniform(true), _blockName(blockName) {}
ShaderParameter::~ShaderParameter() {}

bool ShaderParameter::hasState() const { return _hasState; }
bool ShaderParameter::hasUniform() const { return _hasUniform; }
bool ShaderParameter::isBlockUniform() const { return _blockUniform; }

const std::string &ShaderParameter::getUniformName() const { return _uniformName; }
const std::string &ShaderParameter::getBlockName() const { return _blockName; }

void ShaderParameter::enable(Shader *shader) {}
void ShaderParameter::disable(Shader *shader) {}

const void* ShaderParameter::getUniformData() const { return 0; }
