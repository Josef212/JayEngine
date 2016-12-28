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


	//TODO: change import to bool, in mesh importer too
	void importTexture(const char* fileName, ResourceTexture* resTex); //NOTE: Import from extern format like png
	bool loadTexture(ResourceTexture* resTex);
};

#endif // !__IMPORTERTEXTURE_H__