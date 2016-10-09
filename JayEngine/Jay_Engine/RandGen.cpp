#include "RandGen.h"


RandGen::RandGen()
{
	lgc = new math::LCG();
}

RandGen::~RandGen()
{
	RELEASE(lgc);
}

uint32 RandGen::getRandInt()
{
	return lgc->Int();
}

int	RandGen::getRandInt(int min, int max)
{
	return lgc->Int(min, max);
}

float RandGen::getRandFloat()
{
	return lgc->Float();
}

float RandGen::getRandFloat(float min, float max)
{
	return lgc->Float(min, max);
}