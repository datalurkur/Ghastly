#include <Render/Font.h>

Font::Font(): _material(0) {

}

Renderable* Font::createRenderable(const std::string &text) {
    float *vertexPointer, *texCoordPointer;
	unsigned int *indexPointer;
    Renderable *textBox;

    int numCharacters;
    int i, characterIndex;

    // TODO - Add some code to split up text into multiple lines
    float xPos, yPos;

    // Determine the number of characters we'll print
    numCharacters = 0;
    for(i = 0; i < (int)text.length(); i++) {
        // Don't count spaces or nonprintable characters
        if(text[i] > 32) {
			numCharacters++;
		}
    }

    // Allocate memory for our vertex and texcoord buffers
    vertexPointer   = (float*)calloc(numCharacters * 4 * 2, sizeof(float));
    texCoordPointer = (float*)calloc(numCharacters * 4 * 2, sizeof(float));
	indexPointer    = (unsigned int*)calloc(numCharacters * 4, sizeof(unsigned int));

    // Now set the actual vertex and texcoords
    xPos = 0.0f;
    yPos = 0.0f;

	characterIndex = 0;
    for(i = 0; i < (int)text.length(); i++) {
        char currentCharacter;
        int characterWidth;

        currentCharacter = text[i];

        // Determine how wide this character should be
        characterWidth = _characterWidth[(int)currentCharacter];

        if(currentCharacter > 32) {
            float textureXMin, textureYMin, textureXMax, textureYMax;
            int textureIndexX, textureIndexY;

            // Compute in the range (0,1) where the texcoords fall
            textureIndexX = currentCharacter / 16;
            textureIndexY = currentCharacter % 16;

            textureXMin = textureIndexX * (_fontWidth  / (float)_textureWidth);
            textureYMin = textureIndexY * (_fontHeight / (float)_textureHeight);

            textureXMax = textureXMin + (characterWidth / (float)_textureWidth);
            textureYMax = textureYMin + ((_fontAscent - _fontDescent) / (float)_textureHeight);

            // Set the vertices
            vertexPointer[(characterIndex*8)  ] = xPos;
            vertexPointer[(characterIndex*8)+1] = yPos + _fontAscent;

            vertexPointer[(characterIndex*8)+2] = xPos;
            vertexPointer[(characterIndex*8)+3] = yPos + _fontDescent;

            vertexPointer[(characterIndex*8)+4] = xPos + characterWidth;
            vertexPointer[(characterIndex*8)+5] = yPos + _fontDescent;

            vertexPointer[(characterIndex*8)+6] = xPos + characterWidth;
            vertexPointer[(characterIndex*8)+7] = yPos + _fontAscent;

            // Set up the texcoords
            texCoordPointer[(characterIndex*8)  ] = textureXMin;
            texCoordPointer[(characterIndex*8)+1] = textureYMin;

            texCoordPointer[(characterIndex*8)+2] = textureXMax;
            texCoordPointer[(characterIndex*8)+3] = textureYMin;

            texCoordPointer[(characterIndex*8)+4] = textureXMax;
            texCoordPointer[(characterIndex*8)+5] = textureYMax;

            texCoordPointer[(characterIndex*8)+6] = textureXMin;
            texCoordPointer[(characterIndex*8)+7] = textureYMax;

			// Set up the indices
			indexPointer[(characterIndex*4)  ] = (characterIndex*4);
			indexPointer[(characterIndex*4)+1] = (characterIndex*4)+1;
			indexPointer[(characterIndex*4)+2] = (characterIndex*4)+2;
			indexPointer[(characterIndex*4)+3] = (characterIndex*4)+3;

			// Move to next character
			characterIndex++;
		}

        xPos += characterWidth;
    }

    // Set up our renderable
    textBox = new Renderable();
    textBox->setVertexPointer(vertexPointer, numCharacters * 4, 2);
    textBox->setTexCoordPointer(texCoordPointer, numCharacters * 4, 2);
	textBox->setIndexPointer(indexPointer, numCharacters * 4);
    textBox->setMaterial(_material);

	// Free pointers
	free(vertexPointer);
	free(texCoordPointer);
	free(indexPointer);

    return textBox;
}

float Font::textWidth(const std::string &text) {
	float totalWidth = 0;
	int i;
	for(i = 0; i < (int)text.length(); i++) {
        if(text[i] >= 32) {
			totalWidth += (float)_characterWidth[(int)text[i]];
		}
    }
	return totalWidth;
}

float Font::textHeight(const std::string &text) {
	return (float)_characterHeight;
}

Font::~Font() {
	if(_material) {
		delete _material;
	}
}
