#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool startEnabled = true);
	~ModuleFileSystem();

	bool init();
	bool cleanUp();

	bool addPath(const char* pathOrZip, const char* mountPoint = NULL);
	bool exist(const char* file);
	bool isDirectory(const char* file);
	bool makeDirectory(const char* dir, const char* mount = NULL);
	const char* getSaveDirectory()const
	{
		return "save/";
	}

	const char* getBasePath();

	uint load(const char* file, const char* path, char** buffer)const;
	uint load(const char* file, char** buffer)const;
	SDL_RWops* load(const char* file)const;
	unsigned int save(const char* file, const char* buffer, unsigned int size)const;
};

#endif // !__MODULEFILESYSTEM_H__