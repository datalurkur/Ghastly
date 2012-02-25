#include <Engine/KeyboardListener.h>

KeyboardEvent::KeyboardEvent(const int key, const int modifier): _key(key), _modifier(modifier) {}
int KeyboardEvent::key() const { return _key; }
int KeyboardEvent::modifier() const { return _modifier; }

KeyboardListener::KeyboardListener() {}
