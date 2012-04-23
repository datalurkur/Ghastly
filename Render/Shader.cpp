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
    
    // Get a list of uniforms and their data
    fetchUniformInformation();
}

void Shader::teardown() {
    _uniformData.clear();

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

const Shader::UniformInfo& Shader::getUniformInfo(const std::string &uniformName) {
    UniformInfoMap::iterator itr;

    itr = _uniformData.find(uniformName);
    ASSERT(itr != _uniformData.end());

    return itr->second;
}

void Shader::enable() {
    ASSERT(_program);
    glUseProgram(_program);
}

void Shader::disable() {
    glUseProgram(0);
}

bool Shader::fetchUniformInformation() {
    int i, bufferSize;
    GLint numUniforms;
    
    GLuint *uniformIndices;
    GLint *uniformOffsets, *uniformSizes, *uniformTypes;
    
    GLchar *buffer;

    // Find out how many uniforms there are
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    // Allocate buffers
    uniformIndices = (GLuint*)malloc(sizeof(GLint) * numUniforms);
    uniformOffsets = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformSizes   = (GLint*)malloc(sizeof(GLint) * numUniforms);
    uniformTypes   = (GLint*)malloc(sizeof(GLint) * numUniforms);
    
    // Construct our array of uniforms (get information about all of them)
    for(i = 0; i < numUniforms; i++) { uniformIndices[i] = i; }
    
    // Get information about the uniforms
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes);
    glGetActiveUniformsiv(_program, numUniforms, uniformIndices, GL_UNIFORM_TYPE, uniformTypes);
    
    // Get the names of each uniform and map it
    bufferSize = 256;
    buffer = (GLchar*)calloc(sizeof(GLchar), bufferSize);
    for(i = 0; i < numUniforms; i++) {
        GLsizei nameSize;
        GLuint uniformByteSize;
        std::string uniformName;
        size_t dotIndex;

        glGetActiveUniformName(_program, uniformIndices[i], bufferSize, &nameSize, buffer);
        uniformName = std::string(buffer);
        dotIndex = uniformName.find_first_of('.');
        if(dotIndex != std::string::npos) {
            uniformName = uniformName.substr(dotIndex+1);
        }
        
        // Figure out the uniform size in bytes
        switch(uniformTypes[i]) {
            case GL_FLOAT_VEC3:   uniformByteSize = sizeof(float) * 3; break;
            case GL_FLOAT_VEC4:   uniformByteSize = sizeof(float) * 4; break;
            case GL_FLOAT_MAT4:   uniformByteSize = sizeof(float) * 16; break;
            case GL_SAMPLER_2D:   uniformByteSize = 1; break;
            default:
                Error("Uniform type " << uniformTypes[i] << " not supported.");
                ASSERT(0);
                break;
        };
        
        Info("Found uniform " << uniformName << " (" << i << ") with offset " << uniformOffsets[i] << " and bytesize " << uniformByteSize);
        _uniformData[uniformName] = UniformInfo(i, uniformOffsets[i], uniformByteSize);
    }
    free(buffer);

    // Clean up
    free(uniformIndices);
    free(uniformOffsets);
    free(uniformSizes);
    free(uniformTypes);

    return true;
}