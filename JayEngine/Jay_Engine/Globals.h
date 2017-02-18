#ifndef __GLOBALS__
#define __GLOBALS__

#include <windows.h>
#include <stdio.h>

enum logType
{
	LOG_STD,		//WHITE - Standar info
	LOG_ERROR,		//RED - Error info
	LOG_WARN,		//YELLOW - Warning info
	LOG_CMD,		//BLUE - Commands info
	LOG_INFO,		//GREEN - Info more important than standard
	LOG_INFO_REM,	//ORANGE - Info even more remarkable than INFO logs	//255-128-0
	LOG_NONE		//??? - Dont really know what this would be usefull for but always better have an error for logs... (Dont really know what I'm doing here xD)
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

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define PI 3.14159265358979323846264338327950288

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;
typedef unsigned __int32 UID;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//Clear texture path to remove any folder path on the string
char* clearTexPath(char* path);

// Configuration -----------
#define TITLE "JayEngine"

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

#define PATH_SETTINGS "Data/Settings/"

#define PATH_ASSETS_MODEL "Data/Assets/fbx/"
#define PATH_ASSETS_TEXTURE "Data/Assets/textures/"

#define PATH_SCENES "Data/Scenes/"
#define PATH_LIBRARY_PREFABS "Data/Library/Prefabs/"
#define PATH_LIBRARY_MESH "Data/Library/Meshes/"
#define PATH_LIBRARY_TEXTURE "Data/Library/Textures/"

#define PATH_SHADERS "Data/Shaders/"

#define PATH_TMP "Data/Library/tmp/"

#define EXTENSION_MESH ".jaymesh"
#define EXTENSION_TEXTURE ".dds"
#define EXTENSION_MATERIAL ".jaymat"
#define EXTENSION_SCENE ".jayscene"
#define EXTENSION_VERTEX_SHADER ".jayvertex"
#define EXTENSION_FRAGTAL_SHADER ".jayfragtal"

#endif //!__GLOBALS__