#include <Base/Log.h>
#include <Base/Matrix4.h>
#include <Render/Renderable.h>
#include <Resource/MaterialManager.h>

// TODO - Move drawmode into the material
Renderable::Renderable():
    _numIndices(0), _viewMatrix(Matrix4::Identity), _transformBuffer(0), _material(0), _drawMode(GL_TRIANGLE_STRIP)
{
    setMaterial(MaterialManager::Get("default"));

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_indexBufferID);
}

Renderable::~Renderable() {
    glDeleteBuffers(1, &_indexBufferID);
    glDeleteVertexArrays(1, &_vao);
    
    if(_transformBuffer) { delete _transformBuffer; }
}

void Renderable::setViewMatrix(const Matrix4 &matrix) {
    _viewMatrix = matrix;
}

const Matrix4& Renderable::getViewMatrix() const {
    return _viewMatrix;
}

void Renderable::setIndexBuffer(const unsigned int numIndices, unsigned int *indices) {
    GLuint byteSize;
    
    _numIndices = numIndices;
    byteSize = _numIndices * sizeof(unsigned int);
   
    enterRenderScope();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, indices, GL_STATIC_DRAW);
    exitRenderScope();
}

void Renderable::setAttribBuffer(const std::string &name, GLuint numElements, GLenum elementType, GLuint elementSize, void *buffer) 
{
    GLuint byteSize;
    GLint bindPoint;
    VertexAttribMap::iterator itr;

    switch(elementType) {
        case GL_FLOAT:          byteSize = sizeof(GLfloat) * numElements * elementSize; break;
        case GL_UNSIGNED_INT:   byteSize = sizeof(GLuint) * numElements * elementSize;  break;
        default:                ASSERT(0);                                              break;
    }
    
    itr = _vertexAttribBuffers.find(name);
    if(itr == _vertexAttribBuffers.end()) {
        GLuint newBuffer;
        glGenBuffers(1, &newBuffer);
        _vertexAttribBuffers[name] = newBuffer;
    }
    
    enterRenderScope();
    glBindBuffer(GL_ARRAY_BUFFER, _vertexAttribBuffers[name]);
    glBufferData(GL_ARRAY_BUFFER, byteSize, buffer, GL_STATIC_DRAW);
    
    bindPoint = _material->getShader()->getAttribLocation(name);
    glVertexAttribPointer(bindPoint, elementSize, elementType, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(bindPoint);
    exitRenderScope();
}

void Renderable::setMaterial(Material *material) {
    _material = material;
    recreateTransformBuffer(material->getShader());
}

void Renderable::setDrawMode(GLenum mode) {
    _drawMode = mode;
}

void Renderable::enterRenderScope() {
    glBindVertexArray(_vao);
}

void Renderable::exitRenderScope() {
    glBindVertexArray(0);
}

void Renderable::render(const Matrix4 &projection, const Matrix4 &modelView) {
    updateTransformBuffer(projection, modelView);
    _transformBuffer->enable();

    if(_material) {
        _material->enable();
    }

    enterRenderScope();
    glDrawElements(_drawMode, _numIndices, GL_UNSIGNED_INT, 0);
    exitRenderScope();

    if(_material) {
        _material->disable();
    }

    _transformBuffer->disable();
}

Renderable* Renderable::OrthoBox(const Vector2 &pos, const Vector2 &dims, bool texCoords, bool normals, float z, Material *material) {
    return OrthoBox(Vector3(pos.x, pos.y, z), Vector2(dims.x, dims.y), texCoords, normals, material);
}

Renderable* Renderable::OrthoBox(const Vector3 &pos, const Vector2 &dims, bool texCoords, bool normals, Material *material) {
    Renderable *renderable = new Renderable();
    renderable->setViewMatrix(Matrix4::MakeTranslation(pos));
    renderable->setMaterial(material);

    Vector3 disp = pos + Vector3(dims.x, dims.y, 0.0f);

    // Determine if the vertex order needs to be flipped to preserve proper winding order
    bool flipped = ((dims.x < 0) != (dims.y < 0));

    // Set the verts
    if(flipped) {
        float verts[4 * 3] = {
            pos.x,  disp.y, pos.z,
            disp.x, disp.y, pos.z,
            disp.x, pos.y,  pos.z,
            pos.x,  pos.y,  pos.z
        };
        renderable->setAttribBuffer("position", 4, GL_FLOAT, 3, &verts[0]);
    } else {
        float verts[4 * 3] = {
            pos.x,  pos.y,  pos.z,
            disp.x, pos.y,  pos.z,
            disp.x, disp.y, pos.z,
            pos.x,  disp.y, pos.z
        };
        renderable->setAttribBuffer("position", 4, GL_FLOAT, 3, &verts[0]);
    }

    // Set the texture coordinates
    if(texCoords && flipped) {
        float texCoords[4 * 2] = {
            0, 1,
            1, 1,
            1, 0,
            0, 0
        };
        renderable->setAttribBuffer("texcoord", 4, GL_FLOAT, 2, &texCoords[0]);
    } else if(texCoords) {
        float texCoords[4 * 2] = {
            0, 0,
            1, 0,
            1, 1,
            0, 1
        };
        renderable->setAttribBuffer("texcoord", 4, GL_FLOAT, 2, &texCoords[0]);
    }

    // Set the normals
    if(normals) {
        float normals[4 * 3] = {
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1
        };
        renderable->setAttribBuffer("normal", 4, GL_FLOAT, 2, &normals[0]);
    }

    // Set the indices
    unsigned int indices[4] = { 0, 1, 3, 2 };
    renderable->setIndexBuffer(4, &indices[0]);

    return renderable;
}

Renderable* Renderable::Sprite(const Vector2 &pos, const Vector2 &dims, const float z, Material *material) {
    Renderable *renderable = Renderable::OrthoBox(pos, dims, true, true, z, material);
    renderable->setMaterial(material);
    return renderable;
}

Renderable* Renderable::Lines(const std::vector<Vector2> &verts) {
    Renderable *renderable;
    float *vertexBuffer;
    unsigned int *indexBuffer;
    unsigned int size;
    unsigned int i;

    size = (unsigned int)verts.size();

    renderable = new Renderable();

    vertexBuffer = new float[size * 3];
    indexBuffer = new unsigned int[size];

    for(i=0; i < size; i++) {
        vertexBuffer[3*i  ] = verts[i].x;
        vertexBuffer[3*i+1] = verts[i].y;
        vertexBuffer[3*i+2] = 0.0f;

        indexBuffer[i] = i;
    }

    renderable->setIndexBuffer(size, &indexBuffer[0]);
    renderable->setDrawMode(GL_LINE_STRIP);

    delete vertexBuffer;
    delete indexBuffer;

    return renderable;
}

void Renderable::recreateTransformBuffer(Shader *shader) {
    if(_transformBuffer) { delete _transformBuffer; }
    _transformBuffer = new UniformBuffer(shader, "transform");
    updateTransformBuffer(Matrix4::Identity, _viewMatrix);
}

void Renderable::updateTransformBuffer(const Matrix4 &projection, const Matrix4 &modelView) {
    _transformBuffer->setParameter("projection_matrix", projection.ptr());
    _transformBuffer->setParameter("modelview_matrix", (modelView * _viewMatrix).ptr());
}
