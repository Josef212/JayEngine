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
	_LOG("---Hardware information:  ---");
	_LOG("CPU cores: %d.", info.cpuCores);
	_LOG("CPU cache size: %dKb.", info.cpuCacheSize);
	_LOG(" System RAM: %dMb.", info.ram);
	_LOG("Caps:");
	if (info.hasAVX)
		_LOG("HasAVX");
	if (info.hasAltiVec)
		_LOG("HasAltiVec");
	if (info.hasMMX)
		_LOG("HasMMX");
	if (info.hasRDTSC)
		_LOG("HasRDTSC");
	if (info.hasSSE)
		_LOG("HasSSE");
	if (info.hasSSE2)
		_LOG("HasSSE2");
	if (info.hasSSE3)
		_LOG("HasSSE3");
	if (info.hasSSE41)
		_LOG("HasSSE41");
	if (info.hasSSE42)
		_LOG("HasSSE42");
	if (info.has3DNow)
		_LOG("Has3DNow");
}

void HrdInfo::setCaps()
{
	if (info.hasAVX)
		str.append("AVX ");
	if (info.hasAltiVec)
		str.append("AltiVec ");
	if (info.hasMMX)
		str.append("MMX ");
	if (info.hasRDTSC)
		str.append("RDTSC ");
	if (info.hasSSE)
		str.append("SSE ");
	if (info.hasSSE2)
		str.append("SSE2 ");
	if (info.hasSSE3)
		str.append("SSE3 ");
	if (info.hasSSE41)
		str.append("SSE41 ");
	if (info.hasSSE42)
		str.append("SSE42 ");
	if (info.has3DNow)
		str.append("3DNow");
}

const char* HrdInfo::getCaps()
{
	return str.c_str();
}