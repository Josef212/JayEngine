#ifndef __RESOURCETEXTURE_H__
#define __RESOURCETEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture(UID uuid);
	virtual ~ResourceTexture();

	bool loadToMemory();
	bool removeFromMemory();
};

#endif // !__RESOURCETEXTURE_H__