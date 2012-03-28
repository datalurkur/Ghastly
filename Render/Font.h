#ifndef FONT_H
#define FONT_H

#include <Base/Vector2.h>
#include <Render/Texture.h>

class TTFManager;

class Font {
public:
	Font();
	virtual ~Font();

protected:
	int _fontWidth;
	int _fontHeight;

	int _textureWidth;
	int _textureHeight;

	int _characterWidth[256];
	int _characterHeight;

	int _fontAscent;
	int _fontDescent;
	int _fontLineSkip;

	Texture *_texture;

    friend class TTFManager;
};

#endif