#ifndef __IMPORTER_TEXTURE__
#define __IMPORTER_TEXTURE__

#include "Importer.h"
#include <string>

class Resource;
class ResourceTexture;

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool LoadResource(Resource* resource)override;

	bool Import(const char* originalFile, const char* path, std::string& exportedFile, UID& resUID);
	bool ImportBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID);

	bool LoadChequers(ResourceTexture* res);

};

#endif // !__IMPORTER_TEXTURE__