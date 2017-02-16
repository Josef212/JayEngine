#ifndef __HRDINFO__
#define __HRDINFO__

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

	void SetInfo();
	void LogHrdInfo();
	Hrd* GetInfo() { return &info; }
	void SetCaps();
	const char* GetCaps();
};

#endif // !__HRDINFO__