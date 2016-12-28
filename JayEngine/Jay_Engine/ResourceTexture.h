#ifndef __RESOURCETEXTURE_H__
#define __RESOURCETEXTURE_H__

#include "Resource.h"

/**
	TODO: Image format?????????????
	Image info???????????
	Probably when is start using shaders if i use normal maps or anything like that format will be important
*/

class ResourceTexture : public Resource
{
public:
	ResourceTexture(UID uuid);
	virtual ~ResourceTexture();

	bool loadToMemory()override;
	bool removeFromMemory()override;

	uint textureGlID = 0;
};

#endif // !__RESOURCETEXTURE_H__