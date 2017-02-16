#include "HrdInfo.h"

HrdInfo::HrdInfo()
{
}


HrdInfo::~HrdInfo()
{
}

void HrdInfo::SetInfo()
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
	SetCaps();
}

void HrdInfo::LogHrdInfo()
{
	_LOG(LOG_STD,"---Hardware information:  ---");
	_LOG(LOG_STD,"CPU cores: %d.", info.cpuCores);
	_LOG(LOG_STD,"CPU cache size: %dKb.", info.cpuCacheSize);
	_LOG(LOG_STD," System RAM: %dMb.", info.ram);
	_LOG(LOG_STD,"Caps:");
	if (info.hasAVX)
		_LOG(LOG_STD, "HasAVX");
	if (info.hasAltiVec)
		_LOG(LOG_STD, "HasAltiVec");
	if (info.hasMMX)
		_LOG(LOG_STD, "HasMMX");
	if (info.hasRDTSC)
		_LOG(LOG_STD, "HasRDTSC");
	if (info.hasSSE)
		_LOG(LOG_STD, "HasSSE");
	if (info.hasSSE2)
		_LOG(LOG_STD, "HasSSE2");
	if (info.hasSSE3)
		_LOG(LOG_STD, "HasSSE3");
	if (info.hasSSE41)
		_LOG(LOG_STD, "HasSSE41");
	if (info.hasSSE42)
		_LOG(LOG_STD, "HasSSE42");
	if (info.has3DNow)
		_LOG(LOG_STD, "Has3DNow");
}

void HrdInfo::SetCaps()
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

const char* HrdInfo::GetCaps()
{
	return str.c_str();
}