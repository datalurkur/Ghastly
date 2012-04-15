#include <Base/Assertion.h>
#include <Render/Shader.h>
#include <Render/GLHelper.h>

Shader::UniformInfo::UniformInfo() {}
Shader::UniformInfo::UniformInfo(GLuint i, GLint o, GLint s): index(i), offset(o), size(s) {}
Shader::UniformInfo::UniformInfo(const UniformInfo &other): index(other.index), offset(other.offset), size(other.size) {}

Shader::Shader():
    _program(0), _vertexShader(0), _geometryShader(0), _fragmentShader(0)
{
}

Shader::~Shader() {
    teardown();
}

GLuint Shader::compile(const char *shaderProgramData, GLenum type) {
    GLuint shader;
    GLint compileStatus;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderProgramData, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if(!compileStatus) {
        GLchar *buffer;
        GLsizei bufferLength, actualLength;
        
        bufferLength=512;
        buffer = (GLchar*)calloc(sizeof(GLchar), bufferLength);
        glGetShaderInfoLog(shader, bufferLength, &actualLength, buffer);
        Error("Shader failed to compile: " << buffer);
        free(buffer);
        
        ASSERT(0);   
    }

    return shader;
}

GLuint Shader::getUniformLocation(const std::string &uniformName) {
    GLint location;

    ASSERT(_program);
    location = glGetUniformLocation(_program, uniformName.c_str());

    if(location == -1) {
        Error("Failed to locate uniform " << uniformName);
        ASSERT(0);
    }

    return (GLuint)location;
}

GLuint Shader::getAttribLocation(const std::string &attribName) {
    GLint location;

    ASSERT(_program);
    location = glGetAttribLocation(_program, attribName.c_str());

    if(location == -1) {
        Error("Failed to locate attrib " << attribName);
        ASSERT(0);
    }

    return (GLuint)location;
}

void Shader::setup(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
    GLint linkStatus;

    // Create the shader program
    if(!_program) {
        _program = glCreateProgram();
    }

    // Create and attach the shaders
    if(vertexShader) {
        _vertexShader = vertexShader;
        glAttachShader(_program, _vertexShader);
    }

    if(geometryShader) {
        _geometryShader = geometryShader;
        glAttachShader(_program, _geometryShader);
    }

    if(fragmentShader) {
        _fragmentShader = fragmentShader;
        glAttachShader(_program, _fragmentShader);
    }

    // Link the program and check its success
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &linkStatus);
    ASSERT(linkStatus);
}

void Shader::teardown() {
    UniformBlockInfoMap::iterator itr;

    // Clear out the UBO information
    for(itr = _uniformBlockData.begin(); itr != _uniformBlockData.end(); itr++) {
        delete itr->second;
    }
    _uniformBlockData.clear();

    // Detach and delete the shaders
    if(_vertexShader) {
        glDetachShader(_program, _vertexShader);
        glDeleteShader(_vertexShader);
        _vertexShader = 0;
    }

    if(_geometryShader) {
        glDetachShader(_program, _geometryShader);
        glDeleteShader(_geometryShader);
        _geometryShader = 0;
    }

    if(_fragmentShader) {
        glDetachShader(_program, _fragmentShader);
        glDeleteShader(_fragmentShader);
        _fragmentShader = 0;
    }

    // Delete the shader program
    if(_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

GLuint Shader::getProgramHandle() {
    ASSERT(_program);
    return _program;
}

const Shader::UniformInfo& Shader::getUniformInfo(const std::string &uniformBlockName, const std::string &uniformName) {
    UniformBlockInfoMap::iterator block_itr;
    UniformInfoMap::iterator info_itr;

    block_itr = _uniformBlockData.find(uniformBlockName);
    if(block_itr == _uniformBlockData.end()) {
        _uniformBlockData[uniformBlockName] = new UniformInfoMap();
        ASSERT(fetchUniformBlockInformation(uniformBlockName));
    }

    info_itr = _uniformBlockData[uniformBlockName]->find(uniformName);
    if(info_itr == _uniformBlockData[uniformBlockName]->end()) {
        Error("Uniform " << uniformName << " not found in uniform block " << uniformBlockName);
        ASSERT(0);
    }

    return info_itr->second;
}

void Shader::enable() {
    ASSERT(_program);
    glUseProgram(_program);
}

void Shader::disable() {
    glUseProgram(0);
}

bool Shader::fetchUniformBlockInformation(const std::string &uniformBlockName) {
    int i;

    GLuint blockIndex;

    GLint numUniforms;
    GLuint *uniformIndices;
    GLint *uniformOffsets, *uniformSizes, *uniformTypes;
    unsigned int bufferSize;
    
    UniformBlockInfoMap::iterator itr;
    UniformInfoMap* uniformBlockInfo;

    // Make sure we can an info map allocated
    itr = _uniformBlockData.find(uniformBlockName);
    ASSERT(itr != _uniformBlockData.end());
    uniformBlockInfo = itr->second;

    // Find the block index
    blockIndex = glGetUniformBlockIndex(_program, uniformBlockName.c_str());
    if(blockIndex == -1) {
        Error("Uniform block " << uniformBlockName << " not found for shader.");
        return false;
    }

    glGetActiveUniformBlockiv(_program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);
    ASSERT(numUniforms > 0);

    uniformIndices = (GLuint*)malloc(sizeof(GLint) * numUniforms);
    uniformOffsets = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformSizes = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformTypes = (GLint*)malloc(sizeof(GLint) * numUniforms);

    glGetActiveUniformBlockiv(_program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)uniformIndices);
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

        (*uniformBlockInfo)[std::string(buffer)] = UniformInfo(uniformIndices[i], uniformOffsets[i], uByteSize);
        free(buffer);
    }

    free(uniformIndices);
    free(uniformOffsets);
    free(uniformSizes);
    free(uniformTypes);

    return true;
}