#ifndef __RESOURCE__
#define __RESOURCE__

#include "Globals.h"
#include <string>
#include "FileParser.h"

enum ResourceType
{
	RESOURCE_UNKNOWN = -1,
	RESOURCE_MESH = 0,
	RESOURCE_TEXTURE,
	RESOURCE_SHADER,
	RESOURCE_SCENE,
	RESOURCE_MATERIAL,
	RESOURCE_AUDIO
};

class Resource
{
public:
	Resource(UID uuid, ResourceType resType = RESOURCE_UNKNOWN) : uuid(uuid), resType(resType)
	{}

	virtual ~Resource()
	{}

	ResourceType GetType()const { return resType; }
	UID GetUID()const { return uuid; }

	const char* GetOriginalFileStr()const { return originalFile.c_str(); }
	const char* GetExportedFileStr()const { return exportedFile.c_str(); }

	bool IsInMemory()const { return instancesInMemory > 0; }

	bool LoadToMemory()
	{
		if (instancesInMemory > 0)
			++instancesInMemory;
		else
			instancesInMemory = LoadInMemory() ? 1 : 0;

		return instancesInMemory > 0; 
	}
	
	virtual bool RemoveFromMemory() { return false; }

	uint CountReferences()const { return instancesInMemory; }

	void AddInstance() { ++instancesInMemory; }
	void RemoveInstance() { --instancesInMemory; }

	virtual void Save(FileParser& file)
	{
		file.AddInt("UID", uuid);
		file.AddInt("type", (int)resType);
		file.AddString("original_file", originalFile.c_str());
		file.AddString("exported_file", exportedFile.c_str());
	}

	virtual void Load(FileParser& file)
	{
		uuid = file.GetInt("UID", 0);
		resType = (ResourceType)file.GetInt("type", -1);
		originalFile = file.GetString("original_file", "???");
		exportedFile = file.GetString("exported_file", "???");
	}

protected:
	virtual bool LoadInMemory() = 0;

public:
	std::string originalFile;
	std::string exportedFile;

protected:
	ResourceType resType = RESOURCE_UNKNOWN;
	UID uuid = 0;

	uint instancesInMemory = 0;
};

#endif // !__RESOURCE__