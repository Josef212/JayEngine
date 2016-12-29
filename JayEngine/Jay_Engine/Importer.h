#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"

class Resource;

class Importer
{
public:
	Importer();
	virtual ~Importer();

	bool loadResource(Resource* resource) { return false; }
};

#endif // !__IMPORTER_H__