#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include "Module.h"
//#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool init(FileParser* conf);
	bool cleanUp();
	bool save(FileParser* sect);
	bool load(FileParser* sect);

	// Play a music file
	bool playMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int loadFx(const char* path);

	// Play a previously loaded WAV
	bool playFx(unsigned int fx, int repeat = 0);

private:

//	Mix_Music*			music;
//	p2List<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO_H__