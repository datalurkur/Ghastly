#include <Base/Base.h>
#include <Base/Log.h>
#include <Base/FileSystem.h>
#include <Render/Texture.h>
#include <Base/Assertion.h>
#include <Render/GLHelper.h>
#include <Resource/TextureManager.h>

void Texture::SavePixelDataToDisk(GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, const std::string &name) {
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
    for(unsigned int i=0; i<w; i++) {
        for(unsigned int j=0; j<h; j++) {
            int offset = (i*h) + j;

            switch(format) {
            case GL_ALPHA:
            case GL_LUMINANCE:
                fileData[18 + (offset*3) + 0] = pixelData[offset];
                fileData[18 + (offset*3) + 1] = pixelData[offset];
                fileData[18 + (offset*3) + 2] = pixelData[offset];
                break;
            case GL_BGRA:
                fileData[18 + (offset*3) + 0] = pixelData[(offset*4) + 0];
                fileData[18 + (offset*3) + 1] = pixelData[(offset*4) + 1];
                fileData[18 + (offset*3) + 2] = pixelData[(offset*4) + 2];
                break;
            case GL_BGR:
                fileData[18 + (offset*3) + 0] = pixelData[(offset*3) + 0];
                fileData[18 + (offset*3) + 1] = pixelData[(offset*3) + 1];
                fileData[18 + (offset*3) + 2] = pixelData[(offset*3) + 2];
                break;
            case GL_RGBA:
                fileData[18 + (offset*3) + 0] = pixelData[(offset*4) + 2];
                fileData[18 + (offset*3) + 1] = pixelData[(offset*4) + 1];
                fileData[18 + (offset*3) + 2] = pixelData[(offset*4) + 0];
                break;
            case GL_RGB:
                fileData[18 + (offset*3) + 0] = pixelData[(offset*3) + 2];
                fileData[18 + (offset*3) + 1] = pixelData[(offset*3) + 1];
                fileData[18 + (offset*3) + 2] = pixelData[(offset*3) + 0];
                break;
            default:
                ASSERT(0);
                break;
            };
        }
    }
    FileSystem::SaveFileData(name + ".tga", fileData, fileSize);
    free(fileData);
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
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        CheckGLErrors();
		disable();
	}
}

void Texture::teardown() {
	if(_ids) {
		glDeleteTextures(_frames, _ids);
		delete [] _ids;
	}
}

void Texture::setPixelData(GLenum internalFormat, GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, bool genMipMaps, unsigned int frame) {
    GLenum target = GL_TEXTURE_2D,
           dataType = GL_UNSIGNED_BYTE;

    enable(frame);

    /*// DEBUG
    static int counter = 0;
    std::string name;
    char buffer[4];
    sprintf_s(buffer, "%i", counter);
    name = "debug_texture_";
    name.append(buffer);
    counter++;
    Texture::SavePixelDataToDisk(format, w, h, pixelData, name);*/

    if(genMipMaps) {
        gluBuild2DMipmaps(target, internalFormat, w, h, format, dataType, (void*)pixelData);
    } else {
        glTexImage2D(target, 0, internalFormat, w, h, 0, format, dataType, pixelData);
    }
    CheckGLErrors();

    disable();
}

void Texture::enable(const unsigned int frame) {
    CheckGLErrors();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, _ids[frame]);
    CheckGLErrors();
}

void Texture::disable() {
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    CheckGLErrors();
}
