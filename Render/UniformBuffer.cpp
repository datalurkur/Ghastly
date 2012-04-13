#include <Render/UniformBuffer.h>
#include <Base/Assertion.h>

UniformBuffer::UniformBuffer(GLuint program):
    _program(program), _blockIndex(0), _blockSize(0), _bufferHandle(0)
{ }

UniformBuffer::~UniformBuffer() {
    teardown();
}

bool UniformBuffer::setup(const std::string &blockName) {
    // Find the block index
    _blockIndex = glGetUniformBlockIndex(_program, blockName.c_str());
    if(_blockIndex == -1) {
        Error("Uniform block " << blockName << " not found for shader.");
        return false;
    }

    // Get the size of the block
    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    // Generate the server-side buffer
    glGenBuffers(1, &_bufferHandle);

    // Bind the buffer object to the uniform block
    glBindBufferBase(GL_UNIFORM_BUFFER, _blockIndex, _bufferHandle);

    // Set up initial state to make sure the buffer has *some* data
    enable();
    uploadBufferData(0);
    disable();

    // Get information about the active uniforms in the uniform block
    fetchUniformData();

    return true;
}

void UniformBuffer::teardown() {
    // Delete the buffer
    if(_bufferHandle) {
        glDeleteBuffers(1, &_bufferHandle);
    }
}

void UniformBuffer::enable() {
    // Bind the buffer to the shader program
    glUniformBlockBinding(_program, _blockIndex, _bufferHandle);
}

void UniformBuffer::disable() {
    glUniformBlockBinding(_program, _blockIndex, 0);
}

void UniformBuffer::setParameter(const std::string &name, const void *data) {
    GLuint index;
    index = getUniformIndex(name);
    uploadPartialBufferData(getUniformOffset(index), getUniformSize(index), data);
}

void UniformBuffer::fetchUniformData() {
    int i;
    GLint numUniforms;
    GLint *uniformIndices;
    unsigned int bufferSize;

    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);
    ASSERT(numUniforms > 0);

    uniformIndices = (GLint*)malloc(sizeof(GLint) * numUniforms);
    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices);

    bufferSize = 128;
    for(i = 0; i < numUniforms; i++) {
        GLchar *buffer;
        GLsizei actualSize;
		GLuint uIndex;
        GLint uOffset;
        GLint uSize;

		uIndex = uniformIndices[i];

        // Get the uniform name for this index and set it appropriately
        buffer = (GLchar*)calloc(sizeof(GLchar), bufferSize);
        glGetActiveUniformName(_program, uIndex, bufferSize, &actualSize, buffer);
        _uniformIndices[std::string(buffer)] = uIndex;
        free(buffer);

        // Get the uniform offset and size
        glGetActiveUniformsiv(_program, 1, &uIndex, GL_UNIFORM_OFFSET, &uOffset);
        _uniformOffsets[uIndex] = uOffset;
        glGetActiveUniformsiv(_program, 1, &uIndex, GL_UNIFORM_SIZE, &uSize);
        _uniformSizes[uIndex] = uSize;
    }

    free(uniformIndices);
}

GLuint UniformBuffer::getUniformIndex(const std::string &name) {
    std::map<std::string,GLuint>::iterator itr;

    itr = _uniformIndices.find(name);
    ASSERT(itr != _uniformIndices.end());

    return itr->second;
}

GLint UniformBuffer::getUniformOffset(GLuint index) {
    std::map<GLuint,GLint>::iterator itr;

    itr = _uniformOffsets.find(index);
    ASSERT(itr != _uniformOffsets.end());

    return itr->second;
}

GLint UniformBuffer::getUniformSize(GLuint index) {
    std::map<GLuint,GLint>::iterator itr;

    itr = _uniformSizes.find(index);
    ASSERT(itr != _uniformSizes.end());

    return itr->second;
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
