#ifndef FONT_H
#define FONT_H

#include <Base/Vector2.h>
#include <Render/Material.h>
#include <Render/Renderable.h>

class TTFManager;

class Font {
public:
    enum Alignment {
        TOP_LEFT = 0,
        LEFT,
        BOTTOM_LEFT,
        TOP,
        CENTER,
        BOTTOM,
        TOP_RIGHT,
        RIGHT,
        BOTTOM_RIGHT
    };

public:
	Font();
	virtual ~Font();

    Renderable* createRenderable(const std::string &text, const Vector2 &maxDims, Alignment textAlignment);
    Renderable* createRenderable(const std::list<std::string> &subStrings, const Vector2 &maxDims, Alignment textAlignment);
    void populateBuffers(char currentCharacter, unsigned int characterIndex, int xOffset, int yOffset, float *vertexPointer, float *texCoordPointer, unsigned int *indexPointer);

	int textWidth(const std::string &text);
	int textHeight();

	Material *getMaterial() const;

    void splitAtWidth(const std::string &text, int maxWidth, std::list<std::string> &subStrings, bool clobberWords = false);
    int getSizeInPrintableChars(const std::list<std::string> &strings);

protected:
    int getAlignedX(const std::string &text, const Vector2 &maxDims, Alignment alignment);
    int getAlignedY(int numSubStrings, const Vector2 &maxDims, Alignment alignment);
 
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
