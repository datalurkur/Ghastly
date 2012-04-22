#include <Base/Assertion.h>
#include <SDL2/SDL.h>

void CheckSDLErrorsFL(const char* file, int line) {
    const char *error;

    error = SDL_GetError();

    if(*error != '\0') {
        Error("SDL error at " << file << ":" << line << " - " << error);
        ASSERT(false);
        SDL_ClearError();
    }
}
