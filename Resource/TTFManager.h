#ifndef TTFMANAGER_H
#define TTFMANAGER_H

#include <SDL/SDL_ttf.h>
#include <Render/Font.h>
#include <Resource/ResourceManager.h>

class TTFManager;

class TTFManager: public ResourceManager<Font, TTFManager> {
public:
    static void Setup();
    static void Teardown();

protected:
    static void DoLoad(const std::string &name, Font *font);
    static const std::string LoadDirectory;
	friend class ResourceManager<Font, TTFManager>;

private:
	static void collectFontInformation(TTF_Font *ttfFont, Font *font);
	static void computeCharacterWidths(TTF_Font *ttfFont, Font *font);
	static void createFontTexture(TTF_Font *ttfFont, Font *font, const std::string &name, const Color4& fontColor);
	static int getAlphaForPixel(SDL_Surface *surface, int x, int y);
    static std::string TTFLoadPath();
};

#endif
