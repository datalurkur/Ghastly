#ifndef BASE_H
#define BASE_H

// TODO - Set this up as a project target property, so that it can be toggled on and off more easily
#define DEBUG 1

// String libs
#include <string>
#include <sstream>

// Standard libs
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// IO libs
#include <iostream>
#include <fstream>

// Math libs
#include <algorithm>
#include <cmath>

// Standard data structure libs
#include <stack>
#include <queue>
#include <map>
#include <list>
#include <vector>

// Platform-specific defines
#define PLATFORM_APPLE 0
#define PLATFORM_WIN32 1
#define PLATFORM_LINUX 2

#if defined(__APPLE__) && defined(__MACH__)
# define SYS_PLATFORM PLATFORM_APPLE
#elif defined( __WIN32__ ) || defined( _WIN32 )
# define SYS_PLATFORM PLATFORM_WIN32
#else
# define SYS_PLATFORM PLATFORM_LINUX
#endif

#include <GLEW/GL/glew.h>

// SDL
#include <SDL2/SDL.h>

#if SYS_PLATFORM == PLATFORM_WIN32
# define WIN32_LEAN_AND_MEAN
# define sleep(seconds) Sleep(seconds*1000)
#endif

#if SYS_PLATFORM != PLATFORM_WIN32
# define sprintf_s(buffer, buffer_size, stringbuffer, ...) (sprintf(buffer, stringbuffer, __VA_ARGS__))
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

template <typename T>
void split_string(const std::string &string, const std::string &splitter, T &tokens) {
    size_t token_begin, token_end, splitter_size;

    splitter_size = splitter.length();
    
    token_end = 0;
    while((token_begin = string.find(splitter, token_end)) != std::string::npos) {
        tokens.push_back(string.substr(token_end, token_begin - token_end));
        token_end = token_begin + splitter_size;
    }
}

#endif
