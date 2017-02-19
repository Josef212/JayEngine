#ifndef __IMPORTER__
#define __IMPORTER__

class Resource;

class Importer
{
public:
	Importer()
	{}
	virtual ~Importer()
	{}

	virtual bool LoadResource(Resource* resource) = 0;
};

#endif // !__IMPORTER__