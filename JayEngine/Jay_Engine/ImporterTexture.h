#ifndef __IMPORTER_TEXTURE__
#define __IMPORTER_TEXTURE__

#include <string>

class ResourceTexture;

class ImporterTexture
{
public:
	ImporterTexture();
	virtual ~ImporterTexture();

	bool Import(const char* originalFile, std::string& exportedFile, UID& resUID);
	bool ImportBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID);

	bool LoadResource(ResourceTexture* resource);
};

#endif // !__IMPORTER_TEXTURE__