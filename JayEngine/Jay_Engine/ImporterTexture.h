#ifndef __IMPORTER_TEXTURE__
#define __IMPORTER_TEXTURE__

#include "Importer.h"
#include <string>

class ResourceTexture;

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool Import(const char* originalFile, std::string& exportedFile, UID& resUID);
	bool ImportBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID);

	bool LoadResource(Resource* resource)override;
};

#endif // !__IMPORTER_TEXTURE__