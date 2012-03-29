#include <Resource/TTFManager.h>
#include <Resource/TextureManager.h>
#include <Base/Base.h>

const std::string TTFManager::LoadDirectory = "Font";

void TTFManager::Setup() {
	TTF_Init();
	ResourceManager::Setup();
}

void TTFManager::Teardown() {
	TTF_Quit();
	ResourceManager::Teardown();
}

void TTFManager::DoLoad(const std::string &name, Font *font) {
	std::string fullName;
	int fontSize;

	fullName = LoadPath() + name;

	// TODO - Allow font size specification
	fontSize = 12;
    TTF_Font *ttfFont = TTF_OpenFont(fullName.c_str(), fontSize);
	ASSERT(ttfFont);

	collectFontInformation(ttfFont, font);
	computeCharacterWidths(ttfFont, font);
	createFontTexture(ttfFont, font, fullName + "glyph");
    
    TTF_CloseFont(ttfFont);
}

void TTFManager::collectFontInformation(TTF_Font *ttfFont, Font *font) {
    font->_fontWidth       = 0;
	font->_fontHeight      = TTF_FontHeight(ttfFont);

	font->_fontAscent      = TTF_FontAscent(ttfFont);
	font->_fontDescent     = TTF_FontDescent(ttfFont);

	font->_fontLineSkip    = TTF_FontLineSkip(ttfFont);
	font->_characterHeight = font->_fontAscent - font->_fontDescent;
}

void TTFManager::computeCharacterWidths(TTF_Font *ttfFont, Font *font) {
	int minX, maxX, minY, maxY, advance;

	// Compute character widths
    for( int i = 0; i < 256; ++i ) {
        TTF_GlyphMetrics(ttfFont, static_cast<Uint16>(i), &minX, &maxX, &minY, &maxY, &advance);

        // Set the widths of nonprintable characters to 0
        font->_characterWidth[i] = ((i >= 32) ? advance : 0);

		// Keep track of the widest character, for spacing
        if(font->_characterWidth[i] > font->_fontWidth) {
            font->_fontWidth = advance;
        }
    }
	
    // Set tab width
    font->_characterWidth[9] = font->_characterWidth[32] * 4;
}

void TTFManager::createFontTexture(TTF_Font *ttfFont, Font *font, const std::string &name) {
	int texelCount;
	unsigned char *texels;
	
    int xPos, yPos;
    int currentASCII;

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface *renderedLetter;

    Texture *texture;

	// Compute texture information
	for(font->_textureWidth  = 1; font->_textureWidth  < (font->_fontWidth  * 16); font->_textureWidth  <<= 1) {}
	for(font->_textureHeight = 1; font->_textureHeight < (font->_fontHeight * 16); font->_textureHeight <<= 1) {}

    texelCount = font->_textureWidth * font->_textureHeight;
	texels = (unsigned char *)calloc(texelCount, sizeof(unsigned char));
    
	// Go character by character
	currentASCII = -1;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            currentASCII++;
            if (currentASCII == 0) { continue; }

			// Print the character to an SDL surface
            char letter[2] = {currentASCII, 0};
            renderedLetter = TTF_RenderText_Blended(ttfFont, letter, white);

			// Iterate over each pixel in the SDL surface, copying the data by hand to the texels
			xPos = j * font->_fontWidth;
            yPos = i * font->_fontHeight;
            for (int w = 0; w < renderedLetter->w; w++) {
                for (int h = 0; h < renderedLetter->h; h++) {
					texels[((yPos + h) * font->_textureWidth) + (xPos + w)] = getAlphaForPixel(renderedLetter, w, renderedLetter->h - 1 - h);
                }
            }
            
			// Get ready to print a new character
            SDL_FreeSurface(renderedLetter);
        }
    }

	// Make sure this font doesn't already have a material, for some weird reason
	ASSERT(!font->_material);

	// Create the texture and populate it with texture data
	texture = new Texture();
    texture->setup();
	texture->setPixelData(GL_RGBA, GL_UNSIGNED_BYTE, font->_textureWidth, font->_textureHeight, texels);

    font->_material = new Material();
    font->_material->setTexture(texture);

    free(texels);
}

int TTFManager::getAlphaForPixel(SDL_Surface *surface, int x, int y) {
	SDL_PixelFormat *pixelFormat;
	Uint8 *pixelPointer;
	Uint32 pixel;
	Uint8 alpha;

	pixelFormat  = surface->format;
	pixelPointer = (Uint8*)surface->pixels + (pixelFormat->BytesPerPixel * x) + (surface->pitch * y);

	ASSERT(pixelFormat->BitsPerPixel != 8);

	pixel = *(Uint32*)pixelPointer;
	alpha = (pixel &  pixelFormat->Amask);
	alpha = (alpha >> pixelFormat->Ashift);
	alpha = (alpha >> pixelFormat->Aloss);

	return (int)alpha;
}
