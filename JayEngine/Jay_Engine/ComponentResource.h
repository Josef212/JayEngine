#ifndef __COMPONENT_RESOURCE__
#define __COMPONENT_RESOURCE__

#include "Globals.h"

class Resource;
class FileParser;

class ComponentResource
{
public:
	virtual ~ComponentResource();

	virtual bool SetResource(UID res) = 0;
	virtual const Resource* GetResource()const;
	UID GetResourceUID()const;

protected:
	void OnSaveRes(FileParser& file)const;
	void OnLoadRes(FileParser* file);

protected:
	Resource* resource = nullptr;

};

#endif // !__COMPONENT_RESOURCE__