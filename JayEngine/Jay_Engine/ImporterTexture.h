#ifndef __IMPORTERTEXTURE_H__
#define __IMPORTERTEXTURE_H__

#include "Importer.h"
#include <string>

class ResourceTexture;

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool import(const char* originalFile, std::string& exportedFile, UID& resUID);
	bool importBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID);

	bool loadResource(ResourceTexture* resource);
};

#endif // !__IMPORTERTEXTURE_H__