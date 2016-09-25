#include "HrdInfo.h"

HrdInfo::HrdInfo()
{
}


HrdInfo::~HrdInfo()
{
}

void HrdInfo::setInfo()
{
	info.platform = SDL_GetPlatform();
	info.cpuCacheSize = SDL_GetCPUCacheLineSize();
	info.cpuCores = SDL_GetCPUCount();
	info.ram = SDL_GetSystemRAM();
	info.hasAVX = SDL_HasAVX();
	info.hasAltiVec = SDL_HasAltiVec();
	info.hasMMX = SDL_HasMMX();
	info.hasRDTSC = SDL_HasRDTSC();
	info.hasSSE = SDL_HasSSE();
	info.hasSSE2 = SDL_HasSSE2();
	info.hasSSE3 = SDL_HasSSE3();
	info.hasSSE41 = SDL_HasSSE41();
	info.hasSSE42 = SDL_HasSSE42();
	info.has3DNow = SDL_Has3DNow();
	SDL_GetVersion(&info.sdlVersion);
	info.sdlRevison = SDL_GetRevision();
	setCaps();
}

void HrdInfo::logHrdInfo()
{
	LOG("---Hardware information:  ---");
	LOG("CPU cores: %d.", info.cpuCores);
	LOG("CPU cache size: %dKb.", info.cpuCacheSize);
	LOG(" System RAM: %dMb.", info.ram);
	LOG("Caps:");
	if (info.hasAVX)
		LOG("HasAVX");
	if (info.hasAltiVec)
		LOG("HasAltiVec");
	if (info.hasMMX)
		LOG("HasMMX");
	if (info.hasRDTSC)
		LOG("HasRDTSC");
	if (info.hasSSE)
		LOG("HasSSE");
	if (info.hasSSE2)
		LOG("HasSSE2");
	if (info.hasSSE3)
		LOG("HasSSE3");
	if (info.hasSSE41)
		LOG("HasSSE41");
	if (info.hasSSE42)
		LOG("HasSSE42");
	if (info.has3DNow)
		LOG("Has3DNow");
}

void HrdInfo::setCaps()
{
	if (info.hasAVX)
		str.append("HasAVX, ");
	if (info.hasAltiVec)
		str.append("HasAltiVec, ");
	if (info.hasMMX)
		str.append("HasMMX, ");
	if (info.hasRDTSC)
		str.append("HasRDTSC, ");
	if (info.hasSSE)
		str.append("HasSSE, ");
	if (info.hasSSE2)
		str.append("HasSSE2, ");
	if (info.hasSSE3)
		str.append("HasSSE3, ");
	if (info.hasSSE41)
		str.append("HasSSE41, ");
	if (info.hasSSE42)
		str.append("HasSSE42, ");
	if (info.has3DNow)
		str.append("Has3DNow");
}

const char* HrdInfo::getCaps()
{
	return str.c_str();
}