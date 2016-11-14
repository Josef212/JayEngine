#ifndef __RESOURCEMATERIAL_H__
#define __RESOURCEMATERIAL_H__

#include "Resource.h"
class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(uint UID);
	virtual ~ResourceMaterial();
};

#endif // !__RESOURCEMATERIAL_H__