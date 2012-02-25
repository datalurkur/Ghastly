#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

class TextureManager;

class Texture {
public:
    Texture();
    virtual ~Texture();

	void setup(const unsigned int frames = 1);
	void teardown();
    void setPixelData(GLenum internalFormat, GLenum format, const unsigned int w, const unsigned int h, unsigned char *pixelData, const unsigned int frame = 0);

	void enable(const unsigned int frame = 0);
	void disable();

protected:
	GLuint *_ids;
	unsigned int _frames;

	friend class TextureManager;
};

#endif
