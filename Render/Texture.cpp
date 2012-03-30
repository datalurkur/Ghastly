#include <Base/Base.h>
#include <Base/Log.h>
#include <Base/FileSystem.h>
#include <Render/Texture.h>
#include <Base/Assertion.h>

void Texture::SavePixelDataToDisk(GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, const std::string &name) {
    
}

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
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
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

void Texture::setPixelData(GLenum internalFormat, GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, unsigned int frame) {
    enable(frame);

    // Print the pixel data to file
    unsigned int fileSize = 18 + (3 * w * h);
    char *fileData = (char*)calloc(fileSize, sizeof(char));
    fileData[2] = 2; // Uncompressed RGB
    fileData[12] = (w & 0x00ff);
    fileData[13] = (w & 0xff00) / 256;
    fileData[14] = (h & 0x00ff);
    fileData[15] = (h & 0xff00) / 256;
    fileData[16] = 24; // 24-bit bitmap
    // Pixel data begins at offset 18
    for(int i=0; i<w; i++) {
        for(int j=0; j<h; j++) {
            int offset = (i*h) + j;
            fileData[18 + (offset*3) + 0] = pixelData[offset];
            fileData[18 + (offset*3) + 1] = 0;
            fileData[18 + (offset*3) + 2] = 0;
        }
    }
    FileSystem::SaveFileData("temp_font.tga", fileData, fileSize);
    free(fileData);

    GLenum dataType = GL_UNSIGNED_BYTE,
           target   = GL_TEXTURE_2D;
    gluBuild2DMipmaps(target, internalFormat, w, h, format, dataType, (void*)pixelData);
    //glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, pixelData);
    disable();
    setup(frame);
}

void Texture::enable(const unsigned int frame) {
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, _ids[frame]);
}

void Texture::disable() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
