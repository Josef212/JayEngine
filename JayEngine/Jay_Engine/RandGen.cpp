#include "RandGen.h"


RandGen::RandGen()
{
	//lgc = new math::LCG();
}

RandGen::~RandGen()
{
	RELEASE(lgc);
}

uint32 RandGen::getRandInt()
{
	return 0;
	//return lgc->Int();
}

int	RandGen::getRandInt(int min, int max)
{
	return 0;
	//return lgc->Int(min, max);
}

float RandGen::getRandFloat()
{
	return 0.0f;
	//return lgc->Float();
}

float RandGen::getRandFloat(float min, float max)
{
	return 0.0f;
	//return lgc->Float(min, max);
}