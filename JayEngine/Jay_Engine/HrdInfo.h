#ifndef __HRDINFO_H__
#define __HRDINFO_H__

//Actually will provide some more info not only harware

#include "Globals.h"
#include "SDL\include\SDL.h"
#include <string>

struct Hrd
{
	const char* platform;
	const char* sdlRevison;
	int cpuCacheSize; //Kb
	int cpuCores;
	int ram; //Mb
	bool hasAVX, hasAltiVec, hasMMX, hasRDTSC, hasSSE, hasSSE2, hasSSE3, hasSSE41, hasSSE42, has3DNow;
	SDL_version sdlVersion;
};

class HrdInfo
{
private:
	Hrd info;

	std::string str;

public:
	HrdInfo();
	~HrdInfo();

	void setInfo();
	void logHrdInfo();
	Hrd* getInfo() { return &info; }
	void setCaps();
	const char* getCaps();
};

#endif // !__HRDINFO_H__