#include <Base/Base.h>
#include <Base/Log.h>
#include <Render/Texture.h>

Texture::Texture():
	_ids(0), _frames(1)
{
}

Texture::~Texture() {
	teardown();
}

void Texture::setup(const unsigned int frames) {
	_frames = frames;

	_ids = new GLuint[_frames];
	glGenTextures(_frames, _ids);

	for(unsigned int i=0; i<_frames; i++) {
		enable(i);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0);
		disable();
	}
}

void Texture::teardown() {
	if(_ids) {
		glDeleteTextures(_frames, _ids);
		delete [] _ids;
	}
}

void Texture::setPixelData(GLenum internalFormat, GLenum format, const unsigned int w, const unsigned int h, unsigned char *pixelData, const unsigned int frame) {
    enable(frame);
    gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, w, h, format, GL_UNSIGNED_BYTE, pixelData);
    //glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, pixelData);
    disable();
}

void Texture::enable(const unsigned int frame) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _ids[frame]);
}

void Texture::disable() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
