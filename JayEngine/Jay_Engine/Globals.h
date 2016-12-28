#ifndef __GLOBALS_H__
#define __GLOBALS_H__

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

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

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
#define SETTINGS_PATH "Data/Settings/"

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

#define DEFAULT_FB_PATH "Data/Assets/fbx/"
#define DEFAULT_TEXTURES_PATH "Data/Assets/textures/"

#define DEFAULT_SCENE_SAVE_PATH "Data/Scenes/"
#define DEFAULT_PREF_SAVE_PATHS "Data/Library/Prefabs/"
#define DEFAULT_MESH_SAVE_PATH "Data/Library/Meshes/"
#define DEFAULT_TEXTURE_SAVE_PATH "Data/Library/Textures/"

#define DEFAULT_TMP_DIR "Data/Library/tmp/"

#define MESH_EXTENSION ".jaymesh"
#define TEXTURE_EXTENSION ".dds"
#define MATERIAL_EXTENSION ".jaymat"
#define SCENE_EXTENSION ".jayscene"

#endif