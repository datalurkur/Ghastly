#include <Render/UniformBuffer.h>
#include <Base/Assertion.h>
#include <Render/GLHelper.h>
#include <Render/Shader.h>

IndexPool UniformBuffer::BlockIndices(GL_MAX_UNIFORM_BUFFER_BINDINGS);

UniformBuffer::UniformBuffer(Shader *shader, const std::string &blockName):
    _shader(shader), _blockName(blockName), _bindPoint(0), _blockIndex(0), _blockSize(0), _bufferHandle(0)
{
    ASSERT(setup());
}

UniformBuffer::~UniformBuffer() {
    teardown();
}

bool UniformBuffer::setup() {
    GLuint shaderProgram = _shader->getProgramHandle();

    _bindPoint = (GLuint)BlockIndices.allocate();

    // Generate the server-side buffer
    glGenBuffers(1, &_bufferHandle);

    // Find the block index
    _blockIndex = glGetUniformBlockIndex(shaderProgram, _blockName.c_str());
    if(_blockIndex == -1) {
        Error("Uniform block " << _blockName << " not found for shader.");
        return false;
    }

    // Get the size of the block
    glGetActiveUniformBlockiv(shaderProgram, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    // Finish creation of the buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
    glBufferData(GL_UNIFORM_BUFFER, _blockSize, 0, GL_DYNAMIC_DRAW);

    // Bind the buffer object to the uniform block
    glBindBufferBase(GL_UNIFORM_BUFFER, _bindPoint, _bufferHandle);
    glUniformBlockBinding(shaderProgram, _blockIndex, _bindPoint);

    return true;
}

void UniformBuffer::teardown() {
    // Delete the buffer
    if(_bufferHandle) {
        glDeleteBuffers(1, &_bufferHandle);
    }
    BlockIndices.free((int)_bindPoint);
}

void UniformBuffer::enable() {
    // Bind the buffer to the shader program
    glUniformBlockBinding(_shader->getProgramHandle(), _blockIndex, _bindPoint);
}

void UniformBuffer::disable() {
    glUniformBlockBinding(_shader->getProgramHandle(), _blockIndex, 0);
}

void UniformBuffer::setParameter(const std::string &name, const void *data) {
    Shader::UniformInfo info = _shader->getUniformInfo(_blockName, name);

    uploadPartialBufferData(info.offset, info.size, data);
}

void UniformBuffer::uploadPartialBufferData(unsigned int offset, unsigned int size, const void *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::uploadBufferData(const void *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
    glBufferData(GL_UNIFORM_BUFFER, _blockSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
