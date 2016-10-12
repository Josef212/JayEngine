#include "Globals.h"
#include "Application.h"
#include "ModuleManager.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ModuleManager::ModuleManager(bool startEnabled) : Module(startEnabled)
{
	name.assign("module_manager");
}


ModuleManager::~ModuleManager()
{
}
