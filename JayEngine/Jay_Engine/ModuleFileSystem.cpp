#include "Application.h"
#include "Globals.h"
#include "ModuleFileSystem.h"

#include "PhysFS\include\physfs.h"
#include "SDL\include\SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "FileSystem: Creation.");
	name.assign("module_filesystem");

	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	AddPath(".");
}


ModuleFileSystem::~ModuleFileSystem()
{
	_LOG(LOG_STD, "FileSystem: Destroying.");
	PHYSFS_deinit();
}


bool ModuleFileSystem::Init(FileParser* conf)
{
	_LOG(LOG_STD, "FileSystem: init.");
	bool ret = true;

	/*for (pugi::xml_node path = config.child("path"); path; path = path.next_sibling("path"))
		addPath(path.child_value());*/

	//Old, might be used when swap editor save dir and game saves.
	//char* writePath = SDL_GetPrefPath(app->getOrganitzation(), app->getTitle());

	char* writePath = SDL_GetBasePath(); //Set write dir at the same as working dir.

	if (PHYSFS_setWriteDir(writePath) == 0)
	{
		SDL_Log("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		_LOG(LOG_INFO, "Writing directory is %s\n", writePath);
		AddPath(writePath, GetSaveDirectory());
	}

	AddPath("Data");

	if (!Exist("Library"))
		MakeDirectory("Library", "Data");

	if (!Exist("Library/Meshes"))
		MakeDirectory("Library/Meshes", "Data");

	if (!Exist("Library/Textures"))
		MakeDirectory("Library/Textures", "Data");

	if (!Exist("Library/Prefabs"))
		MakeDirectory("Library/Prefabs", "Data");

	if (!Exist("Library/tmp"))
		MakeDirectory("Library/tmp", "Data");

	_LOG(LOG_INFO, "PHYS_FS base path: %s", GetBasePath());

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	_LOG(LOG_STD, "FileSystem: CleanUp.");
	return true;
}

bool ModuleFileSystem::AddPath(const char* pathOrZip, const char* mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
	{
		_LOG(LOG_ERROR, "File System error while adding a path or zip(%s): %s.", pathOrZip, PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::Exist(const char* file)
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::IsDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::MakeDirectory(const char* dir, const char* mount)
{
	bool ret = false;

	if (!dir)
		return ret;

	char newDir[128];

	if (mount)
		sprintf_s(newDir, 128, "%s/%s", mount, dir);
	else
		sprintf_s(newDir, 128, dir);

	if (PHYSFS_mkdir(newDir) == 0)
	{
		_LOG(LOG_ERROR, "Could not make dir: %s. PhsyFs error: %s", newDir, PHYSFS_getLastError());
	}
	else
	{
		_LOG(LOG_INFO, "Just created a dir: %s.", newDir);
		ret = true;
	}

	return ret;
}

uint ModuleFileSystem::GetFilesOnDir(const char* dir, std::vector<std::string>& files)
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != nullptr; ++it)
	{
		files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

uint ModuleFileSystem::Load(const char* file, const char* path, char** buffer)const
{
	if (!file)
		return 0;

	char realPath[256];
	if (path)
		sprintf_s(realPath, 256, "%s%s", path, file);
	else
		strcpy_s(realPath, 256, file);

	return Load(realPath, buffer);
}

uint ModuleFileSystem::Load(const char* file, char** buffer)const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);
			{
				if (readed != size)
				{
					_LOG(LOG_ERROR, "File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
					RELEASE(buffer);
				}
				else
				{
					ret = (uint)readed;
				}
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			_LOG(LOG_ERROR, "File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		_LOG(LOG_ERROR, "File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

int closeSdlRwops(SDL_RWops *rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

SDL_RWops* ModuleFileSystem::Load(const char* file)const
{
	char* buffer;
	int size = Load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != nullptr)
			r->close = closeSdlRwops;

		return r;
	}
	else
		return nullptr;
}

unsigned int ModuleFileSystem::Save(const char* file, const char* buffer, unsigned int size)const
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			_LOG(LOG_ERROR, "File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fsFile) == 0)
			_LOG(LOG_ERROR, "File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		_LOG(LOG_ERROR, "File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

const char* ModuleFileSystem::GetBasePath()
{
	return PHYSFS_getBaseDir();
}

void ModuleFileSystem::NormalizePath(char* path)
{
	if (!path)
		return;

	int len = strlen(path);
	for (uint i = 0; i < len; ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
		/*else
			path[i] = tolower(path[i]);*/
	}
}

void ModuleFileSystem::NormalizePath(std::string& path)
{
	for (std::string::iterator it = path.begin(); it < path.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		/*else
			*it = tolower(*it);*/
	}
}

void ModuleFileSystem::SplitPath(const char* originalPath, std::string* path, std::string* file, std::string* extension)
{
	if (!originalPath)
		return;

	std::string fullPath(originalPath);
	NormalizePath(fullPath);
	int lastSeparator = fullPath.find_last_of("\\/");
	int dot = fullPath.find_last_of(".");

	if (path)
	{
		if (lastSeparator < fullPath.length())
			*path = fullPath.substr(0, lastSeparator + 1);
		else
			path->clear();
	}

	if (file)
	{
		if (lastSeparator < fullPath.length())
			*file = fullPath.substr(lastSeparator + 1);
		else
			*file = fullPath;
	}

	if (extension)
	{
		if (dot < fullPath.length())
			*extension = fullPath.substr(dot + 1);
		else
			extension->clear();
	}
}