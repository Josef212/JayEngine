#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool startEnabled = true);
	~ModuleFileSystem();

	bool init(FileParser* conf);
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

	uint getFilesOnDir(const char* dir, std::vector<std::string>& files);

	uint load(const char* file, const char* path, char** buffer)const;
	uint load(const char* file, char** buffer)const;
	SDL_RWops* load(const char* file)const;
	unsigned int save(const char* file, const char* buffer, unsigned int size)const;

	void splitPath(const char* originalPath, std::string* path, std::string* file = NULL, std::string* extension = NULL);
	void normalizePath(char* path);
	void normalizePath(std::string& path);
};

#endif // !__MODULEFILESYSTEM_H__