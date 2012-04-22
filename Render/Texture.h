#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL_image.h>
#include <Base/Base.h>

class TextureManager;

class Texture {
public:
    static void SavePixelDataToDisk(GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, const std::string &name);

public:
    Texture();
    virtual ~Texture();

    void setup(const unsigned int frames = 1);
    void teardown();
    void setPixelData(GLenum internalFormat, GLenum format, unsigned int w, unsigned int h, unsigned char *pixelData, bool genMipMaps = true, unsigned int frame = 0);

    void enable(const unsigned int frame = 0);
    void disable();

protected:
    GLuint *_ids;
    unsigned int _frames;

    friend class TextureManager;
    friend class TextureParameter;
};

#endif
