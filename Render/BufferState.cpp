#include <Render/BufferState.h>
#include <Base/Assertion.h>

// Buffer State
// ============
BufferState::BufferState(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, GLenum bufferType):
    _numElements(numElements), _elementType(elementType), _elementSize(elementSize), _bufferType(bufferType), _useAttribs(false)
{
    unsigned int totalByteSize;

    switch(elementType) {
    case GL_FLOAT:
        totalByteSize = numElements * elementSize * sizeof(float);
        break;
    default:
        ASSERT(0);
        break;
    };

    _buffer = malloc(totalByteSize);
    ASSERT(_buffer);
    memcpy(_buffer, buffer, totalByteSize);
}

BufferState::~BufferState() {
    if(_buffer) { free(_buffer); }
}

void BufferState::preRender() {
    if(_useAttribs) {
        glEnableVertexAttribArray(_attribBindPoint);
        glVertexAttribPointer(_attribBindPoint, _elementSize, _elementType, GL_FALSE, 0, _buffer);
    } else {
        glEnableClientState(_bufferType);

        switch(_bufferType) {
        case GL_VERTEX_ARRAY:        glVertexPointer(_elementSize, _elementType, 0, _buffer); break;
        case GL_NORMAL_ARRAY:        glNormalPointer(_elementType, 0, _buffer); break;
        case GL_TEXTURE_COORD_ARRAY: glTexCoordPointer(_elementSize, _elementType, 0, _buffer); break;
        case GL_COLOR_ARRAY:         glColorPointer(_elementSize, _elementType, 0, _buffer); break;
        default:                     ASSERT(0); break;
        };
    }
}

void BufferState::postRender() {
    if(!_useAttribs) {
        glDisableClientState(_bufferType);
    } else {
        glDisableVertexAttribArray(_attribBindPoint);
    }
}

void BufferState::setupAttribState(GLuint attribBindPoint) {
    _useAttribs = true;
    _attribBindPoint = attribBindPoint;
}

bool BufferState::AreAttribsEnabled() {
    // Actually check to see if shaders and shader attribs are enabled
    return true;
}

BufferState *BufferState::VertexBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader) {
    GLuint location;
    BufferState *bufferState;

    location = shader->getAttribLocation("position");
    bufferState = new BufferState(numElements, elementType, elementSize, buffer, GL_VERTEX_ARRAY);

    if(AreAttribsEnabled()) {
        bufferState->setupAttribState(location);
    }

    return bufferState;
}

BufferState *BufferState::NormalBuffer(GLuint numElements, GLenum elementType, void *buffer, Shader *shader) {
    GLuint location;
    BufferState *bufferState;

    location = shader->getAttribLocation("normal");
    bufferState = new BufferState(numElements, elementType, 2, buffer, GL_NORMAL_ARRAY);

    if(AreAttribsEnabled()) {
        bufferState->setupAttribState(location);
    }

    return bufferState;
}

BufferState *BufferState::TexCoordBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader) {
    GLuint location;
    BufferState *bufferState;

    location = shader->getAttribLocation("texcoord");
    bufferState = new BufferState(numElements, elementType, elementSize, buffer, GL_TEXTURE_COORD_ARRAY);

    if(AreAttribsEnabled()) {
        bufferState->setupAttribState(location);
    }

    return bufferState;
}

BufferState *BufferState::ColorBuffer(GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer, Shader *shader) {
    GLuint location;
    BufferState *bufferState;

    location = shader->getAttribLocation("color");
    bufferState = new BufferState(numElements, elementType, elementSize, buffer, GL_COLOR_ARRAY);

    if(AreAttribsEnabled()) {
        bufferState->setupAttribState(location);
    }

    return bufferState;
}