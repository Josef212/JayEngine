#ifndef __RESOURCESCENE_H__
#define __RESOURCESCENE_H__

#include "Resource.h"

class ResourceScene : public Resource
{
public:
	ResourceScene(UID uuid);
	virtual ~ResourceScene();
};

#endif