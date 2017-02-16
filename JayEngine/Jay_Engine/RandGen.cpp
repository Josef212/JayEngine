#include "RandGen.h"


RandGen::RandGen()
{
	lgc = new math::LCG();
}

RandGen::~RandGen()
{
	RELEASE(lgc);
}

uint32 RandGen::GetRandInt()
{
	return lgc->Int();
}

int	RandGen::GetRandInt(int min, int max)
{
	return lgc->Int(min, max);
}

float RandGen::GetRandFloat()
{
	return lgc->Float();
}

float RandGen::GetRandFloat(float min, float max)
{
	return lgc->Float(min, max);
}