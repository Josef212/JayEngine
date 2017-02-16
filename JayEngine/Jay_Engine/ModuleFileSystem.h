#ifndef __MODULEFILESYSTEM__
#define __MODULEFILESYSTEM__

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool startEnabled = true);
	~ModuleFileSystem();

	bool Init(FileParser* conf)override;
	bool CleanUp()override;

	bool AddPath(const char* pathOrZip, const char* mountPoint = NULL);
	bool Exist(const char* file);
	bool IsDirectory(const char* file);
	bool MakeDirectory(const char* dir, const char* mount = NULL);
	const char* GetSaveDirectory()const
	{
		return "save/";
	}

	const char* GetBasePath();

	uint GetFilesOnDir(const char* dir, std::vector<std::string>& files);

	uint Load(const char* file, const char* path, char** buffer)const;
	uint Load(const char* file, char** buffer)const;
	SDL_RWops* Load(const char* file)const;
	uint Save(const char* file, const char* buffer, unsigned int size)const;

	void SplitPath(const char* originalPath, std::string* path, std::string* file = NULL, std::string* extension = NULL);
	void NormalizePath(char* path);
	void NormalizePath(std::string& path);
};

#endif // !__MODULEFILESYSTEM__