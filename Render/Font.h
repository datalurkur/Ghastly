#ifndef FONT_H
#define FONT_H

#include <Render/Texture.h>

class FontManager;

class Font {
public:
	Font();
	virtual ~Font();

private:
	Texture *_texture;
    friend class FontManager;
};

#endif