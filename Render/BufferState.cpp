#include <Render/BufferState.h>
#include <Base/Assertion.h>

// Buffer State
// ============
BufferState::BufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer, GLenum bufferType):
    _numElements(numElements), _elementType(elementType), _elementSize(elementSize), _bufferType(bufferType)
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
    glEnableClientState(_bufferType);
}

void BufferState::postRender() {
    glDisableClientState(_bufferType);
}

// Vertex Buffer State
// ===================
VertexBufferState::VertexBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer):
    BufferState(numElements, elementType, elementSize, buffer, GL_VERTEX_ARRAY) {}

void VertexBufferState::preRender() {
    BufferState::preRender();
    glVertexPointer(_elementSize, _elementType, 0, _buffer);
}

// Tex Coord Buffer State
// ======================
TexCoordBufferState::TexCoordBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer):
    BufferState(numElements, elementType, elementSize, buffer, GL_TEXTURE_COORD_ARRAY) {}

void TexCoordBufferState::preRender() {
    BufferState::preRender();
    glTexCoordPointer(_elementSize, _elementType, 0, _buffer);
}

// Color Buffer State
// ==================
ColorBufferState::ColorBufferState(unsigned int numElements, GLenum elementType, unsigned int elementSize, void *buffer):
    BufferState(numElements, elementType, elementSize, buffer, GL_COLOR_ARRAY) {}

void ColorBufferState::preRender() {
    BufferState::preRender();
    glColorPointer(_elementSize, _elementType, 0, _buffer);
}

// Normal Buffer State
// ===================
NormalBufferState::NormalBufferState(unsigned int numElements, GLenum elementType, void *buffer):
    BufferState(numElements, elementType, 3, buffer, GL_NORMAL_ARRAY) {}

void NormalBufferState::preRender() {
    BufferState::preRender();
    glNormalPointer(_elementType, 0, _buffer);
}
