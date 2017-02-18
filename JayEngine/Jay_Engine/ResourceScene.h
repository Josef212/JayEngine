#ifndef __RESOURCESCENE__
#define __RESOURCESCENE__

#include "Resource.h"

class ResourceScene : public Resource
{
public:
	ResourceScene(UID uuid);
	virtual ~ResourceScene();

	void Save(FileParser& file)override;
	void Load(FileParser& file)override;

	bool LoadInMemory()override;
};

#endif //!__RESOURCESCENE__