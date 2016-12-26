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

	addPath(".");
}


ModuleFileSystem::~ModuleFileSystem()
{
	_LOG(LOG_STD, "FileSystem: Destroying.");
	PHYSFS_deinit();
}


bool ModuleFileSystem::init(FileParser* conf)
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
		addPath(writePath, getSaveDirectory());
	}

	addPath("Data");

	if (!exist("Library"))
		makeDirectory("Library", "Data");

	if (!exist("Library/Meshes"))
		makeDirectory("Library/Meshes", "Data");

	if (!exist("Library/Textures"))
		makeDirectory("Library/Textures", "Data");

	if (!exist("Library/Prefabs"))
		makeDirectory("Library/Prefabs", "Data");

	if (!exist("Library/tmp"))
		makeDirectory("Library/tmp", "Data");

	_LOG(LOG_INFO, "PHYS_FS base path: %s", getBasePath());

	return ret;
}

bool ModuleFileSystem::cleanUp()
{
	_LOG(LOG_STD, "FileSystem: CleanUp.");
	return true;
}

bool ModuleFileSystem::addPath(const char* pathOrZip, const char* mountPoint)
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

bool ModuleFileSystem::exist(const char* file)
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::isDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::makeDirectory(const char* dir, const char* mount)
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

uint ModuleFileSystem::getFilesOnDir(const char* dir, std::vector<std::string>& files)
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != NULL; ++it)
	{
		files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

uint ModuleFileSystem::load(const char* file, const char* path, char** buffer)const
{
	if (!file)
		return 0;

	char realPath[256];
	if (path)
		sprintf_s(realPath, 256, "%s%s", path, file);
	else
		strcpy_s(realPath, 256, file);

	return load(realPath, buffer);
}

uint ModuleFileSystem::load(const char* file, char** buffer)const
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

SDL_RWops* ModuleFileSystem::load(const char* file)const
{
	char* buffer;
	int size = load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = closeSdlRwops;

		return r;
	}
	else
		return NULL;
}

unsigned int ModuleFileSystem::save(const char* file, const char* buffer, unsigned int size)const
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

const char* ModuleFileSystem::getBasePath()
{
	return PHYSFS_getBaseDir();
}