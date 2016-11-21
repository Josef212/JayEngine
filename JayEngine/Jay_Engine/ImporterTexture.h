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
	//TODO: change import to bool, in mesh importer too
	void importTexture(const char* fileName, ResourceTexture* resTex); //NOTE: Import from extern format 
};

#endif // !__IMPORTERTEXTURE_H__