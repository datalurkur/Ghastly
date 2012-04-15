#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <Base/Base.h>
#include <Base/Vector2.h>
#include <Base/Matrix4.h>
#include <Render/Material.h>
#include <Render/BufferState.h>
#include <Render/UniformBuffer.h>
#include <SDL/SDL_opengl.h>

class Renderable {
public:
	Renderable();
	virtual ~Renderable();

	void setViewMatrix(const Matrix4 &matrix);
	const Matrix4& getViewMatrix() const;

    void addRenderState(GenericRenderState *renderState);
    void clearRenderStates();
	void setIndexPointer(unsigned int *indices, const unsigned int numIndices);

	void setMaterial(Material *material);
    void setDrawMode(GLenum mode);

	void render(const Matrix4 &projection, const Matrix4 &modelView);

public:
	static Renderable* OrthoBox(const Vector2 &pos, const Vector2 &dims, bool texCoords, bool normals, float z = 0.0f);
    static Renderable* OrthoBox(const Vector3 &pos, const Vector2 &dims, bool texCoords, bool normals);
    static Renderable* Sprite(const Vector2 &pos, const Vector2 &dims, const float z, Material *material);
    static Renderable* Lines(const std::vector<Vector2> &verts);

private:
    typedef std::list<GenericRenderState*> RenderStateList;

private:
    void recreateTransformBuffer(Shader *shader);
    void updateTransformBuffer(const Matrix4 &projection, const Matrix4 &modelView);

private:
	Matrix4 _viewMatrix;
    UniformBuffer *_transformBuffer;

	Material *_material;
    RenderStateList _renderStates;
	unsigned int *_indexPointer;
	unsigned int _numIndices;

	GLenum _drawMode;
};

typedef std::list<Renderable*> RenderableList;

#endif
