#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include "Globals.h"
#include "Math.h"

class RandGen
{
public:
	RandGen();
	~RandGen();

	uint32 getRandInt();
	int getRandInt(int min, int max);
	float getRandFloat();
	float getRandFloat(float min, float max);

private:
	LCG* lgc = NULL;
};

#endif // !__RANDGEN_H__