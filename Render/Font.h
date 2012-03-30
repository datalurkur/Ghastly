#ifndef FONT_H
#define FONT_H

#include <Base/Vector2.h>
#include <Render/Material.h>
#include <Render/Renderable.h>

class TTFManager;

class Font {
public:
	Font();
	virtual ~Font();

    Renderable* createRenderable(const std::string &text);
	float textWidth(const std::string &text);
	float textHeight(const std::string &text);

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

	Material *_material;

    friend class TTFManager;
};

#endif
