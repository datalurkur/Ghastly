#include <Resource/TTFManager.h>
#include <Resource/TextureManager.h>
#include <Base/Base.h>
#include <Base/PropertyMap.h>

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
    char *fileData;
	unsigned int fileSize;
    
    std::string fontName, fontFullPath;
	int fontSize;
    Color4 fontColor;

    PropertyMap *pMap;
    std::list<std::string> keys;
    std::list<std::string>::iterator itr;

    // Load the PMap
	fileSize = FileSystem::GetFileData(LoadPath() + name, &fileData);
    if(fileSize == 0) {
        Error("Failed to load " << name);
        return;
    }
    pMap = new PropertyMap(fileData);
    free(fileData);

    // Parse the PMap
    pMap->getKeys(keys);
    for(itr = keys.begin(); itr != keys.end(); itr++) {
        if(*itr == "color4") {
            pMap->getValue(*itr, fontColor);
        } else if(*itr == "size") {
            pMap->getValue(*itr, fontSize);
        } else if(*itr == "ttf") {
            pMap->getValue(*itr, fontName);
        }
    }
    delete pMap;

    // Build the font
    fontFullPath = TTFLoadPath() + fontName;
    TTF_Font *ttfFont = TTF_OpenFont(fontFullPath.c_str(), fontSize);
	ASSERT(ttfFont);

	collectFontInformation(ttfFont, font);
	createFontTexture(ttfFont, font, name + "glyph", fontColor);

    TTF_CloseFont(ttfFont);
}

void TTFManager::collectFontInformation(TTF_Font *ttfFont, Font *font) {
    font->_fontWidth       = 0;
	font->_fontHeight      = TTF_FontHeight(ttfFont);

	font->_fontAscent      = TTF_FontAscent(ttfFont);
	font->_fontDescent     = TTF_FontDescent(ttfFont);

	font->_fontLineSkip    = TTF_FontLineSkip(ttfFont);
	font->_characterHeight = font->_fontAscent - font->_fontDescent;

	computeCharacterWidths(ttfFont, font);
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

void TTFManager::createFontTexture(TTF_Font *ttfFont, Font *font, const std::string &name, const Color4& fontColor) {
	int texelCount;
	unsigned char *texels;

    int xPos, yPos;
    int currentASCII;

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface *renderedLetter;

	// Compute texture information
	for(font->_textureWidth  = 1; font->_textureWidth  < (font->_fontWidth  * 16); font->_textureWidth  <<= 1) {}
	for(font->_textureHeight = 1; font->_textureHeight < (font->_fontHeight * 16); font->_textureHeight <<= 1) {}

    texelCount = font->_textureWidth * font->_textureHeight;
	texels = (unsigned char *)calloc(texelCount, sizeof(unsigned char));

	// Go character by character
	currentASCII = -1;
	renderedLetter = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            currentASCII++;
            if (currentASCII == 0) { continue; }

			// Print the character to an SDL surface
            char letter[2] = {currentASCII, 0};
            renderedLetter = TTF_RenderText_Blended(ttfFont, letter, white);
            ASSERT(renderedLetter);

			// Iterate over each pixel in the SDL surface, copying the data by hand to the texels
			xPos = j * font->_fontWidth;
            yPos = i * font->_fontHeight;
            for (int w = 0; w < renderedLetter->w; w++) {
                for (int h = 0; h < renderedLetter->h; h++) {
                    int alpha = getAlphaForPixel(renderedLetter, w, renderedLetter->h - 1 - h);
					texels[((yPos + h) * font->_textureWidth) + (xPos + w)] = (unsigned char)alpha;
                }
            }

			// Get ready to print a new character
            SDL_FreeSurface(renderedLetter);
            renderedLetter = 0;
        }
    }

	// Populate the texture with data
	font->getMaterial()->getTexture()->setPixelData(GL_ALPHA, GL_ALPHA, font->_textureWidth, font->_textureHeight, texels, false);

	// TODO - Figure out how to make this color mutable
	font->_material->setColor(fontColor);

    free(texels);
}

int TTFManager::getAlphaForPixel(SDL_Surface *surface, int x, int y) {
	SDL_PixelFormat *pixelFormat;
	Uint8 *pixelPointer;
	Uint32 pixel;
	Uint32 alpha;

	pixelFormat  = surface->format;
	pixelPointer = (Uint8*)surface->pixels + (pixelFormat->BytesPerPixel * x) + (surface->pitch * y);

	ASSERT(pixelFormat->BitsPerPixel != 8);

	pixel = *(Uint32*)pixelPointer;
	alpha = (pixel &  pixelFormat->Amask);
	alpha = (alpha >> pixelFormat->Ashift);
	alpha = (alpha << pixelFormat->Aloss);

	ASSERT(alpha <= 255);

	return (int)alpha;
}

std::string TTFManager::TTFLoadPath() {
    std::string cleanedPath;
    FileSystem::CleanFilename(LoadPath() + "../TTF/", cleanedPath);
    return cleanedPath;
}
