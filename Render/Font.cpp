#include <Render/Font.h>
#include <Resource/MaterialManager.h>
#include <Resource/ShaderManager.h>
#include <Resource/TextureManager.h>

Font::Font() {
    _material = new Material();
    _glyph = new Texture();
}

Font::~Font() {
    if(_glyph) {
        delete _glyph;
    }

    if(_material) {
        delete _material;
    }
}

void Font::setup() {
    _glyph->setup();

    // TODO - Create a font shader
    _material->setShader(ShaderManager::Get("textured"));
}

void Font::teardown() {
    _glyph->teardown();
}

Renderable* Font::createRenderable(const std::string &text, const Vector2 &maxDims, Alignment textAlignment) {
    std::list<std::string> subStrings;

    // Split the text into substrings based on the maximum width
    splitAtWidth(text, (int)maxDims.w, subStrings);

    // Cull the resultant substrings based on maximum height
    int maxSubStrings = (int)(maxDims.h / _fontLineSkip);
    bool odd = false;
    while(maxSubStrings < (int)subStrings.size()) {
        switch(textAlignment) {
        case TOP_LEFT:
        case TOP:
        case TOP_RIGHT:
            // Shave substrings off the bottom of the list as necessary
            subStrings.pop_back();
            break;
        case BOTTOM_LEFT:
        case BOTTOM:
        case BOTTOM_RIGHT:
            // Shave substrings off the top of the list as necessary
            subStrings.pop_front();
            break;
        case LEFT:
        case CENTER:
        case RIGHT:
            // Shave substrings off both sides of the list
            if(odd) { subStrings.pop_back();  }
            else    { subStrings.pop_front(); }
            odd = !odd;
            break;
        };
    }

    return createRenderable(subStrings, maxDims, textAlignment);
}

Renderable* Font::createRenderable(const std::list<std::string> &subStrings, const Vector2 &maxDims, Alignment textAlignment) {
    float *vertexPointer, *texCoordPointer;
	unsigned int *indexPointer;
    Renderable *textBox;

    int numCharacters;
    int i, characterIndex;

    int xPos, yPos;
    std::list<std::string>::const_reverse_iterator ritr;

    // Determine the number of characters we'll print
    numCharacters = getSizeInPrintableChars(subStrings);

    // Allocate memory for our vertex and texcoord buffers
    vertexPointer   = (float*)calloc(numCharacters * 4 * 2, sizeof(float));
    texCoordPointer = (float*)calloc(numCharacters * 4 * 2, sizeof(float));
	indexPointer    = (unsigned int*)calloc(numCharacters * 4, sizeof(unsigned int));

    // Now set the actual vertex and texcoords
	characterIndex = 0;
    yPos = getAlignedY((int)subStrings.size(), maxDims, textAlignment);
    for(ritr = subStrings.rbegin(); ritr != subStrings.rend(); ritr++) {
        xPos = getAlignedX((*ritr), maxDims, textAlignment);

        for(i = 0; i < (int)(*ritr).length(); i++) {
            char currentCharacter;

            currentCharacter = (*ritr)[i];
            if(currentCharacter > 32) {
                populateBuffers(currentCharacter, characterIndex, xPos, yPos, vertexPointer, texCoordPointer, indexPointer);
			    characterIndex++;
		    }

            xPos += _characterWidth[(int)currentCharacter];
        }

        yPos += _fontLineSkip;
    }

    // Set up our renderable
    textBox = new Renderable();
    textBox->setMaterial(_material);
    textBox->addRenderState(BufferState::VertexBuffer(numCharacters * 4, GL_FLOAT, 2, vertexPointer, _material->getShader()));
    textBox->addRenderState(BufferState::TexCoordBuffer(numCharacters * 4, GL_FLOAT, 2, texCoordPointer, _material->getShader()));
	textBox->setIndexPointer(indexPointer, numCharacters * 4);

	// Free pointers
	free(vertexPointer);
	free(texCoordPointer);
	free(indexPointer);

    return textBox;
}

void Font::populateBuffers(char currentCharacter, unsigned int characterIndex, int xOffset, int yOffset, float *vertexPointer, float *texCoordPointer, unsigned int *indexPointer) {
    float textureXMin, textureYMin, textureXMax, textureYMax;
    int textureIndexX, textureIndexY;

    // Compute in the range (0,1) where the texcoords fall
    textureIndexX = currentCharacter % 16;
    textureIndexY = currentCharacter / 16;

    textureXMin = textureIndexX * (_fontWidth  / (float)_textureWidth);
    textureYMin = textureIndexY * (_fontHeight / (float)_textureHeight);

    textureXMax = textureXMin + (_characterWidth[(int)currentCharacter] / (float)_textureWidth);
    textureYMax = textureYMin + ((_fontAscent - _fontDescent) / (float)_textureHeight);

    // Set the vertices
    vertexPointer[(characterIndex*8) + 0] = (float)xOffset;
    vertexPointer[(characterIndex*8) + 1] = (float)yOffset + _fontAscent;

    vertexPointer[(characterIndex*8) + 2] = (float)xOffset;
    vertexPointer[(characterIndex*8) + 3] = (float)yOffset + _fontDescent;

    vertexPointer[(characterIndex*8) + 4] = (float)xOffset + _characterWidth[(int)currentCharacter];
    vertexPointer[(characterIndex*8) + 5] = (float)yOffset + _fontDescent;

    vertexPointer[(characterIndex*8) + 6] = (float)xOffset + _characterWidth[(int)currentCharacter];
    vertexPointer[(characterIndex*8) + 7] = (float)yOffset + _fontAscent;

    // Set up the texcoords
    texCoordPointer[(characterIndex*8) + 0] = textureXMin;
    texCoordPointer[(characterIndex*8) + 1] = textureYMax;

    texCoordPointer[(characterIndex*8) + 2] = textureXMin;
    texCoordPointer[(characterIndex*8) + 3] = textureYMin;

    texCoordPointer[(characterIndex*8) + 4] = textureXMax;
    texCoordPointer[(characterIndex*8) + 5] = textureYMin;

    texCoordPointer[(characterIndex*8) + 6] = textureXMax;
    texCoordPointer[(characterIndex*8) + 7] = textureYMax;

	// Set up the indices
	indexPointer[(characterIndex*4) + 0] = (characterIndex*4);
	indexPointer[(characterIndex*4) + 1] = (characterIndex*4) + 1;
	indexPointer[(characterIndex*4) + 2] = (characterIndex*4) + 2;
    indexPointer[(characterIndex*4) + 3] = (characterIndex*4) + 3;
}

int Font::textWidth(const std::string &text) {
	int totalWidth = 0;
	int i;
	for(i = 0; i < (int)text.length(); i++) {
        if(text[i] >= 32) {
			totalWidth += _characterWidth[(int)text[i]];
		}
    }
	return totalWidth;
}

int Font::textHeight() {
	return _characterHeight;
}

Material* Font::getMaterial() const {
    return _material;
}

void Font::splitAtWidth(const std::string &text, int maxWidth, std::list<std::string> &subStrings, bool clobberWords) {
    // Split the primary string into substrings based on newlines and carriage returns already present
    std::list<std::string> lines;   
    tokenize_string(text, "\n", lines);

    // Go through the lines and split them up by width
    std::list<std::string>::iterator itr;
    for(itr = lines.begin(); itr != lines.end(); itr++) {
        int totalWidth, i, lastIndex, lastSpace, widthSinceSpace;

        lastSpace  = -1;
        lastIndex  = 0;
        totalWidth = 0;
        widthSinceSpace = 0;
	    for(i = 0; i < (int)(*itr).length(); i++) {
            if((*itr)[i] >= 32) {
                int cWidth;
 
                cWidth = _characterWidth[(int)(*itr)[i]];

                if((*itr)[i] == 32) {
                    lastSpace = i;
                    widthSinceSpace = 0;
                } else {
                    widthSinceSpace += cWidth;
                }

                if(totalWidth + cWidth > maxWidth) {
                    if(lastSpace == -1 || clobberWords) {
                        subStrings.push_back((*itr).substr(lastIndex, i - lastIndex + 1));
                        lastIndex = i;
                        totalWidth = 0;
                    } else {
                        subStrings.push_back((*itr).substr(lastIndex, lastSpace - lastIndex + 1));
                        lastIndex  = lastSpace + 1;
                        totalWidth = widthSinceSpace;
                    }
                }
			    totalWidth += cWidth;
		    }
        }
        if(lastIndex < (int)(*itr).length()) {
            subStrings.push_back((*itr).substr(lastIndex));
        }
    }
}

int Font::getSizeInPrintableChars(const std::list<std::string> &strings) {
    int size;
    std::list<std::string>::const_iterator itr;

    size = 0;
    for(itr = strings.begin(); itr != strings.end(); itr++) {
        int j;
        for(j = 0; j < (int)(*itr).length(); j++) {
            if((*itr)[j] > 32) { size++; }
        }
    }

    return size;
}

int Font::getAlignedX(const std::string &text, const Vector2 &maxDims, Alignment alignment) {
    switch(alignment) {
    case TOP:
    case CENTER:
    case BOTTOM:
        return ((int)maxDims.x - textWidth(text)) / 2;
    case TOP_RIGHT:
    case RIGHT:
    case BOTTOM_RIGHT:
        return (int)maxDims.x - textWidth(text);
    default:
        return 0;
    };
}

int Font::getAlignedY(int numSubStrings, const Vector2 &maxDims, Alignment alignment) {
    switch(alignment) {
    case LEFT:
    case CENTER:
    case RIGHT:
        return ((int)maxDims.y - (textHeight() * numSubStrings)) / 2;
    case TOP_LEFT:
    case TOP:
    case TOP_RIGHT:
        return (int)maxDims.y - (textHeight() * numSubStrings);
    default:
        return 0;
    };
}
