#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

#define _LOG(format, ...) _log(__FILE__, __LINE__, format, __VA_ARGS__);

void _log(const char file[], int line, const char* format, ...);

#ifdef NULL
#undef NULL
#endif
#define NULL  0

#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "JayEngine"
#define ENGINE_VERSION "0.1.1-Alpha"
#define REPOSITORY_URL "https://github.com/Josef212/JayEngine"
#define GITHUB_URL "https://github.com/Josef212"
#define WIKI_URL "https://github.com/Josef212/JayEngine/wiki"
#define RELEASES_URL "https://github.com/Josef212/JayEngine/releases"
#define ISSUES_URL "https://github.com/Josef212/JayEngine/issues"

#endif