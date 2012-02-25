#include <Render/Font.h>

Font::Font():
	_texture(0)
{
}

Font::~Font() {
	if(_texture) {
		delete _texture;
	}
}