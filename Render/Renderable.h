#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Base/Base.h>
#include <Base/Vector2.h>
#include <Base/Matrix4.h>
#include <Render/Material.h>
#include <SDL/SDL_opengl.h>

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

	void setViewMatrix(const Matrix4 &matrix);
	const Matrix4& getViewMatrix() const;

	void setVertexPointer(float *vertexPointer, const unsigned int numVerts, const unsigned int vertSize);
	void setTexCoordPointer(float *texCoordPointer, const unsigned int numTexCoords, const unsigned int texCoordSize);
	void setNormalPointer(float *normalPointer, const unsigned int numNormals);
	void setIndexPointer(unsigned int *indices, const unsigned int numIndices);

	void setMaterial(Material *material);
    void setDrawMode(GLenum mode);

	void render();

public:
	static Renderable* OrthoBox(const Vector2 &pos, const Vector2 &dims, const float z, bool texCoords, bool normals);
    static Renderable* Sprite(const Vector2 &pos, const Vector2 &dims, const float z, Material *mat);
    static Renderable* Lines(const std::vector<Vector2> &verts);

private:
	Matrix4 _viewMatrix;

	Material *_material;

	float *_vertexPointer, *_texCoordPointer, *_normalPointer;
	unsigned int _vertSize, _texCoordSize;

	unsigned int *_indexPointer;
	unsigned int _numIndices;

	GLenum _drawMode;
};

typedef std::list<Renderable*> RenderableList;

#endif
