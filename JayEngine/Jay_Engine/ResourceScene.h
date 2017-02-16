#ifndef __RESOURCESCENE__
#define __RESOURCESCENE__

#include "Resource.h"

class ResourceScene : public Resource
{
public:
	ResourceScene(UID uuid);
	virtual ~ResourceScene();
};

#endif //!__RESOURCESCENE__