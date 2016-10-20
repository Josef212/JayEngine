#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

enum logType
{
	LOG_STD,
	LOG_ERROR,
	LOG_WARN,
	LOG_CMD,
	LOG_AUDIO,
	LOG_CAMERA,
	LOG_EDITOR,
	LOG_FS,
	LOG_IMPORT,
	LOG_INPUT,
	LOG_MANAGER,
	LOG_PHYSICS,
	LOG_REN,
	LOG_WIN
};

#define _LOG(type, format, ...) _log(type, __FILE__, __LINE__, format, __VA_ARGS__);

void _log(logType type, const char file[], int line, const char* format, ...);

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

#define IS_INSIDE(n, maxVal, minVal) (n >= minVal && n <= maxVal) ? (true) : (false)
#define IS_BETWEEN(n, maxVal, minVal) (n > minVal && n < maxVal) ? (true) : (false)

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

#define ASSIMP_DOC_URL "http://www.assimp.org/lib_html/index.html"
#define BULLET_DOC_URL "http://bulletphysics.org/Bullet/BulletFull/"
#define DEVIL_DOC_URL "http://openil.sourceforge.net/docs/"
#define IMGUI_DOC_URL "https://github.com/ocornut/imgui/blob/master/README.md"
#define MATHGEOLIB_DOC_URL "http://clb.demon.fi/MathGeoLib/nightly/usermanual.html"
#define OPENGL_DOC_URL "https://www.opengl.org/documentation/"
#define PHYSFS_DOC_URL "https://icculus.org/physfs/docs/html/index.html"
#define SDL_DOC_URL "https://wiki.libsdl.org/"
#define SDL_MIXER_DOC_URL "https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_frame.html"

#define DEFAULT_FB_PATH "Assets/fbx"
#define DEFAULT_TEXTURES_PATH "Assets/textures"

#endif