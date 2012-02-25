#ifndef BASE_H
#define BASE_H

// String libs
#include <string>
#include <sstream>

// Standard libs
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// IO libs
#include <iostream>

// Math libs
#include <algorithm>
#include <cmath>

// Standard data structure libs
#include <stack>
#include <map>
#include <list>
#include <vector>

#include "SDL/SDL.h"

// Platform-specific defines
#define	PLATFORM_APPLE 0
#define PLATFORM_WIN32 1
#define	PLATFORM_LINUX 2

#if defined(__APPLE__) && defined(__MACH__)
# define SYS_PLATFORM PLATFORM_APPLE
#elif defined( __WIN32__ ) || defined( _WIN32 )
# define SYS_PLATFORM PLATFORM_WIN32
#else
# define SYS_PLATFORM PLATFORM_LINUX
#endif

#if SYS_PLATFORM == PLATFORM_APPLE
# define ASSERT_FUNCTION __asm__("int $03")
#elif SYS_PLATFORM == PLATFORM_WIN32
# define ASSERT_FUNCTION __asm { int 3 }
#else
# include <assert.h>
# define ASSERT(conditional) assert(conditional)
#endif

#ifndef ASSERT
# define ASSERT(conditional) \
    do { \
        if(!(conditional)) { \
			ASSERT_FUNCTION; \
		} \
    } while(false)
#endif

#if SYS_PLATFORM == PLATFORM_WIN32
# include "Windows.h"
# define sleep(seconds) Sleep(seconds*1000)
#elif SYS_PLATFORM == PLATFORM_LINUX
# include <GL/gl.h>
# include <GL/glu.h>
#endif

using namespace std;

// String conversion / manipulation
template <typename T>
bool string_to_decimal(const std::string &string, T &t) {
    std::istringstream stream(string);
    return !(stream >> t).fail();
}

template <typename T>
void tokenize_string(const std::string &string, const std::string &delims, T &tokens) {
    size_t token_begin, token_end;

    // Get the first line
    token_begin = string.find_first_not_of(delims);
    token_end   = string.find_first_of(delims, token_begin);

    // Parse the lines
    while(token_begin != std::string::npos) {
        tokens.push_back(string.substr(token_begin, token_end - token_begin));

        // Get the next line
        token_begin = string.find_first_not_of(delims, token_end);
        token_end   = string.find_first_of(delims, token_begin);
    }
}

#endif
