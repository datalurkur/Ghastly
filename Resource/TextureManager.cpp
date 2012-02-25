#include <Resource/TextureManager.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

const std::string TextureManager::LoadDirectory = "Texture";

void TextureManager::DoLoad(const std::string &name, Texture *texture) {
	std::string fullName;
	SDL_Surface *tSurf;
    GLenum internalFormat, format;

	// Create an SDL surface with the image data
    fullName = LoadPath() + name;
	tSurf = IMG_Load(fullName.c_str());

    ASSERT(SDL_LockSurface(tSurf)==0);

    internalFormat = GL_RGBA;

    switch(tSurf->format->BytesPerPixel) {
#if SYS_PLATFORM == PLATFORM_APPLE
        case 3: format = GL_BGR; break;
        case 4: format = GL_BGRA; break;
#else
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
#endif
        default: ASSERT(0); break;
    };

    // Setup the texture
    texture->setup();
    texture->setPixelData(internalFormat, format, tSurf->w, tSurf->h, (unsigned char*)tSurf->pixels, 0);

	// Destroy the SDL surface    
    SDL_UnlockSurface(tSurf);
	SDL_FreeSurface(tSurf);
}
