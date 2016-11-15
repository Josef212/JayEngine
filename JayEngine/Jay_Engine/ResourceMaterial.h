#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"
class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(UID uuid);
	virtual ~ResourceMaterial();

	bool loadToMemory();
	bool removeFromMemory();
};

#endif // !__RESOURCEMATERIAL_H__