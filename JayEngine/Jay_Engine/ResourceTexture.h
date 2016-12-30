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
	enum Format
	{
		COLOR_INDEX = 0,
		RGB,
		RGBA,
		BGR,
		BGRA,
		LUMINANCE,
		UNKNOWN
	};

public:
	ResourceTexture(UID uuid);
	virtual ~ResourceTexture();

	bool loadToMemory()override;
	bool removeFromMemory()override;

	const char* getFormatStr()const;

public:
	uint width = 0;
	uint height = 0;
	uint depth = 0;
	uint bpp = 0;
	uint mips = 0;
	uint bytes = 0;
	
	uint textureGlID = 0;

	Format format = UNKNOWN;

};

#endif // !__RESOURCETEXTURE_H__