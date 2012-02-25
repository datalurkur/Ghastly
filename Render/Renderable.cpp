#include <Render/Renderable.h>
#include <Base/Log.h>

Renderable::Renderable():
	_viewMatrix(Matrix4::Identity), _material(0),
	_vertexPointer(0), _texCoordPointer(0), _normalPointer(0),
	_vertSize(3), _texCoordSize(2),
	_indexPointer(0), _numIndices(0),
	_drawMode(GL_QUADS)
{
}

Renderable::~Renderable() {
	if(_indexPointer) { free(_indexPointer); }
	if(_vertexPointer) { free(_vertexPointer); }
	if(_texCoordPointer) { free(_texCoordPointer); }
	if(_normalPointer) { free(_normalPointer); }
}

void Renderable::setViewMatrix(const Matrix4 &matrix) {
	_viewMatrix = matrix;
}

const Matrix4& Renderable::getViewMatrix() const {
	return _viewMatrix;
}

void Renderable::setVertexPointer(float *vertexPointer, const unsigned int numVerts, const unsigned int vertSize) {
	size_t byteSize = numVerts * vertSize * sizeof(float);
	_vertexPointer = (float*)malloc(byteSize);
	memcpy(_vertexPointer, vertexPointer, byteSize);
	_vertSize = vertSize;
}

void Renderable::setTexCoordPointer(float *texCoordPointer, const unsigned int numTexCoords, const unsigned int texCoordSize) {
	size_t byteSize = numTexCoords * texCoordSize * sizeof(float);
	_texCoordPointer = (float*)malloc(byteSize);
	memcpy(_texCoordPointer, texCoordPointer, byteSize);
	_texCoordSize = texCoordSize;
}

void Renderable::setNormalPointer(float *normalPointer, const unsigned int numNormals) {
	size_t byteSize = numNormals * 3 * sizeof(float);
	_normalPointer = (float*)malloc(byteSize);
	memcpy(_normalPointer, normalPointer, byteSize);
}

void Renderable::setIndexPointer(unsigned int *indexPointer, const unsigned int numIndices) {
	size_t byteSize = numIndices * sizeof(unsigned int);
	_indexPointer = (unsigned int *)malloc(byteSize);
	memcpy(_indexPointer, indexPointer, byteSize);
	_numIndices = numIndices;
}

void Renderable::setMaterial(Material *material) {
	_material = material;
}

void Renderable::setDrawMode(GLenum mode) {
    _drawMode = mode;
}

void Renderable::render() {
    glPushMatrix();
    glMultMatrixf(_viewMatrix.ptr());

	if(_material) {
		_material->enable();
	}

	if(_indexPointer) {
		if(_vertexPointer) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(_vertSize, GL_FLOAT, 0, _vertexPointer);
		}

		if(_texCoordPointer) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(_texCoordSize, GL_FLOAT, 0, _texCoordPointer);
		}

		if(_normalPointer) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, _normalPointer);
		}
		glDrawElements(_drawMode, _numIndices, GL_UNSIGNED_INT, _indexPointer);
	}

	if(_material) {
		_material->disable();
	}

    glPopMatrix();
}

Renderable* Renderable::OrthoBox(const Vector2 &pos, const Vector2 &dims, const float z, bool texCoords, bool normals) {
	Renderable *renderable = new Renderable();
	renderable->setViewMatrix(Matrix4::MakeTranslation(pos.x, pos.y, z));

	float hW = dims.x / 2.0f,
		  hH = dims.y / 2.0f;

	float verts[4 * 3] = {
		-hW, -hH, 0,
		 hW, -hH, 0,
		 hW,  hH, 0,
		-hW,  hH, 0
	};
	renderable->setVertexPointer(&verts[0], 4, 3);

	if(texCoords) {
		float texCoords[4 * 2] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};
		renderable->setTexCoordPointer(&texCoords[0], 4, 2);
	}

	if(normals) {
		float normals[4 * 3] = {
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1
		};
		renderable->setNormalPointer(&normals[0], 4);
	}

	unsigned int indices[4] = { 0, 1, 2, 3 };
	renderable->setIndexPointer(&indices[0], 4);

	return renderable;
}

Renderable* Renderable::Sprite(const Vector2 &pos, const Vector2 &dims, const float z, Material *mat) {
    Renderable *renderable = Renderable::OrthoBox(pos, dims, z, true, true);
    renderable->setMaterial(mat);
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
    
    renderable->setVertexPointer(&vertexBuffer[0], size, 3);
    renderable->setIndexPointer(&indexBuffer[0], size);
    renderable->setDrawMode(GL_LINE_STRIP);

    delete vertexBuffer;
    delete indexBuffer;

    return renderable;
}
