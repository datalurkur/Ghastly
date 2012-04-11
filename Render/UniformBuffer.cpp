#include <Render/UniformBuffer.h>
#include <Base/Assertion.h>

UniformBuffer::UniformBuffer():
    _blockIndex(0), _blockSize(0), _bufferHandle(0)
{ }

UniformBuffer::~UniformBuffer() {
    teardown();
}

bool UniformBuffer::setup(GLuint program, const std::string &blockName) {
    // Find the block index
    _blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
    if(_blockIndex != -1) {
        Error("Uniform block " << blockName << " not found for shader.");
        return false;
    }

    // Get the size of the block
    glGetActiveUniformBlockiv(program, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    // Generate the server-side buffer
    glGenBuffers(1, &_bufferHandle);

    // Bind the buffer object to the uniform block
    glBindBufferBase(GL_UNIFORM_BUFFER, _blockIndex, _bufferHandle);

    // Bind the buffer to the shader program
    glUniformBlockBinding(program, _blockIndex, _bufferHandle);

    // Set up initial state to make sure the buffer has *some* data
    enable();
    uploadBufferData(0);
    disable();

    // Get information about the active uniforms in the uniform block
    fetchUniformData(program);

    return true;
}

void UniformBuffer::teardown() {
    // Delete the buffer
    if(_bufferHandle) {
        glDeleteBuffers(1, &_bufferHandle);
    }
}

void UniformBuffer::enable() {
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
}

void UniformBuffer::disable() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::setParameter(const std::string &name, const void *data) {
    GLuint index;

    index = getUniformIndex(name);

    enable();
    uploadPartialBufferData(getUniformOffset(index), getUniformSize(index), data);
    disable();
}

void UniformBuffer::fetchUniformData(GLint program) {
    int i;
    GLint numUniforms;
    GLint *uniformIndices;
    unsigned int bufferSize;

    glGetActiveUniformBlockiv(program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);
    ASSERT(numUniforms > 0);

    uniformIndices = (GLint*)malloc(sizeof(GLint) * numUniforms);
    glGetActiveUniformBlockiv(program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices);

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
        glGetActiveUniformName(program, uIndex, bufferSize, &actualSize, buffer);
        _uniformIndices[std::string(buffer)] = uIndex;
        free(buffer);

        // Get the uniform offset and size
        glGetActiveUniformsiv(program, 1, &uIndex, GL_UNIFORM_OFFSET, &uOffset);
        _uniformOffsets[uIndex] = uOffset;
        glGetActiveUniformsiv(program, 1, &uIndex, GL_UNIFORM_SIZE, &uSize);
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
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBuffer::uploadBufferData(const void *data) {
    glBufferData(GL_UNIFORM_BUFFER, _blockSize, data, GL_DYNAMIC_DRAW);
}
