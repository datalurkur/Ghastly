#include <Render/UniformBuffer.h>
#include <Base/Assertion.h>
#include <Render/GLHelper.h>

IndexPool UniformBuffer::BlockIndices(GL_MAX_UNIFORM_BUFFER_BINDINGS);

UniformBuffer::UniformBuffer(GLuint program, const std::string &blockName):
    _program(program), _blockName(blockName), _bindPoint(0), _blockIndex(0), _blockSize(0), _bufferHandle(0)
{ }

UniformBuffer::~UniformBuffer() {
    teardown();
}

bool UniformBuffer::setup() {
    _bindPoint = (GLuint)BlockIndices.allocate();

    // Generate the server-side buffer
    glGenBuffers(1, &_bufferHandle);

    // Find the block index
    _blockIndex = glGetUniformBlockIndex(_program, _blockName.c_str());
    if(_blockIndex == -1) {
        Error("Uniform block " << _blockName << " not found for shader.");
        return false;
    }

    // Get the size of the block
    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    // Finish creation of the buffer object
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferHandle);
    glBufferData(GL_UNIFORM_BUFFER, _blockSize, 0, GL_DYNAMIC_DRAW);

    // Bind the buffer object to the uniform block
    glBindBufferBase(GL_UNIFORM_BUFFER, _bindPoint, _bufferHandle);
    glUniformBlockBinding(_program, _blockIndex, _bindPoint);

    // Get information about the active uniforms in the uniform block
    fetchUniformData();

    CheckGLErrors();

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
    glUniformBlockBinding(_program, _blockIndex, _bindPoint);
}

void UniformBuffer::disable() {
    glUniformBlockBinding(_program, _blockIndex, 0);
}

void UniformBuffer::setParameter(const std::string &name, const void *data) {
    UniformInfo info = getUniformData(name);
    uploadPartialBufferData(info.offset, info.size, data);
}

void UniformBuffer::fetchUniformData() {
    int i;
    GLint numUniforms;
    GLuint *uniformIndices;
    GLint *uniformOffsets, *uniformSizes, *uniformTypes;
    unsigned int bufferSize;

    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);
    ASSERT(numUniforms > 0);

    uniformIndices = (GLuint*)malloc(sizeof(GLint) * numUniforms);
    uniformOffsets = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformSizes = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformTypes = (GLint*)malloc(sizeof(GLint) * numUniforms);

    glGetActiveUniformBlockiv(_program, _blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)uniformIndices);
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes);
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_TYPE, uniformTypes);

    bufferSize = 128;
    for(i = 0; i < numUniforms; i++) {
        GLchar *buffer;
        GLsizei nameSize;
        GLint uByteSize;

        // Get the uniform name for this index and set it appropriately
        buffer = (GLchar*)calloc(sizeof(GLchar), bufferSize);
        glGetActiveUniformName(_program, uniformIndices[i], bufferSize, &nameSize, buffer);

        // Figure out the uniform size in bytes
        switch(uniformTypes[i]) {
        case GL_FLOAT_VEC3:   uByteSize = sizeof(float) * 3; break;
        case GL_FLOAT_VEC4:   uByteSize = sizeof(float) * 4; break;
        case GL_FLOAT_MAT4:   uByteSize = sizeof(float) * 16; break;
        case GL_SAMPLER_2D:   uByteSize = 1; break;
        default:
            Error("Uniform type " << uniformTypes[i] << " not supported.");
            ASSERT(0);
            break;
        };

        _uniformData[std::string(buffer)] = UniformInfo(uniformIndices[i], uniformOffsets[i], uByteSize);
        free(buffer);
    }

    free(uniformIndices);
    free(uniformOffsets);
    free(uniformSizes);
    free(uniformTypes);
}

const UniformBuffer::UniformInfo& UniformBuffer::getUniformData(const std::string &name) {
    std::map<std::string,UniformInfo>::iterator itr;

     itr = _uniformData.find(name);
    ASSERT(itr != _uniformData.end());

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

UniformBuffer::UniformInfo::UniformInfo() {}
UniformBuffer::UniformInfo::UniformInfo(GLuint i, GLint o, GLint s): index(i), offset(o), size(s) {}
UniformBuffer::UniformInfo::UniformInfo(const UniformInfo &other): index(other.index), offset(other.offset), size(other.size) {}