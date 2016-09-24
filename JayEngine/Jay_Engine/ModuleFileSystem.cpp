#include "Application.h"
#include "Globals.h"
#include "ModuleFileSystem.h"

#include "PhysFS\include\physfs.h"
#include "SDL\include\SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(bool startEnabled) : Module(startEnabled)
{
	name.assign("file_system");

	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	addPath(".");
}


ModuleFileSystem::~ModuleFileSystem()
{
	PHYSFS_deinit();
}


bool ModuleFileSystem::awake()
{
	bool ret = true;

	/*for (pugi::xml_node path = config.child("path"); path; path = path.next_sibling("path"))
		addPath(path.child_value());*/

	char* writePath = SDL_GetPrefPath(app->getOrganitzation(), app->getTitle());

	if (PHYSFS_setWriteDir(writePath) == 0)
	{
		SDL_Log("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		LOG("Writing directory is %s\n", writePath);
		addPath(writePath, getSaveDirectory());
	}

	return ret;
}

bool ModuleFileSystem::cleanUp()
{
	return true;
}

bool ModuleFileSystem::addPath(const char* pathOrZip, const char* mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
		SDL_Log("File System error while adding a path or zip(%s): %s\n", pathOrZip, PHYSFS_getLastError());
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

unsigned int ModuleFileSystem::load(const char* file, char** buffer)const
{
	unsigned int ret = 0;

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
					SDL_Log("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
					RELEASE(buffer);
				}
				else
				{
					ret = (uint)readed;
				}
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			SDL_Log("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		SDL_Log("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

int closeSdlRwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
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
			SDL_Log("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		else
			ret = (uint)written;

		if (PHYSFS_close(fsFile) == 0)
			SDL_Log("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		SDL_Log("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

const char* ModuleFileSystem::getBasePath()
{
	return PHYSFS_getBaseDir();
}