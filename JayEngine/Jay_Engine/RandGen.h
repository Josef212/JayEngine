#ifndef __RANDGEN__
#define __RANDGEN__

#include "Globals.h"
#include "Math.h"

class RandGen
{
public:
	RandGen();
	~RandGen();

	uint32 GetRandInt();
	int GetRandInt(int min, int max);
	float GetRandFloat();
	float GetRandFloat(float min, float max);

private:
	LCG* lgc = NULL;
};

#endif // !__RANDGEN__