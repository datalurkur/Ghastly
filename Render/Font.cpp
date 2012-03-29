#include <Render/Font.h>

Font::Font(): _material(0) {

}

Renderable* Font::createRenderable(const std::string &text) {
    float *vertexPointer, *texCoordPointer;
    Renderable *textBox;

    int numCharacters;
    int i;

    // TODO - Add some code to split up text into multiple lines
    float xPos, yPos;

    // Determine the number of characters we'll print
    numCharacters = 0;
    for(i = 0; i < (int)text.length(); i++) {
        // Don't count spaces or nonprintable characters
        if(text[i] > 32) { numCharacters++; }
    }

    // Allocate memory for our vertex and texcoord buffers
    vertexPointer   = (float*)calloc(numCharacters * 4 * 2, sizeof(float));
    texCoordPointer = (float*)calloc(numCharacters * 4 * 2, sizeof(float));

    // Now set the actual vertex and texcoords
    xPos = 0.0f;
    yPos = 0.0f;

    for(i = 0; i < (int)text.length(); i++) {
        char currentCharacter;
        float characterWidth;

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
            vertexPointer[i  ] = xPos;
            vertexPointer[i+1] = yPos + _fontDescent;

            vertexPointer[i+2] = xPos + characterWidth;
            vertexPointer[i+3] = yPos + _fontDescent;

            vertexPointer[i+4] = xPos + characterWidth;
            vertexPointer[i+5] = yPos + _fontAscent;

            vertexPointer[i+6] = xPos;
            vertexPointer[i+7] = yPos + _fontAscent;

            // Set up the texcoords
            texCoordPointer[i  ] = textureXMin;
            texCoordPointer[i+1] = textureYMin;

            texCoordPointer[i+2] = textureXMax;
            texCoordPointer[i+3] = textureYMin;

            texCoordPointer[i+4] = textureXMax;
            texCoordPointer[i+5] = textureYMax;

            texCoordPointer[i+6] = textureXMin;
            texCoordPointer[i+7] = textureYMax;
        }

        xPos += characterWidth;
    }

    // Set up our renderable
    textBox = new Renderable();
    textBox->setVertexPointer(vertexPointer, numCharacters * 4, 2);
    textBox->setTexCoordPointer(texCoordPointer, numCharacters * 4, 2);
    textBox->setMaterial(_material);

    return textBox;
}

Font::~Font() {
	if(_material) {
		delete _material;
	}
}
