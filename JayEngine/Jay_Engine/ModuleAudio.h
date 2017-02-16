#ifndef __MODULEAUDIO__
#define __MODULEAUDIO__

#include "Module.h"
//#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init(FileParser* conf)override;
	bool CleanUp()override;
	bool Save(FileParser* sect)override;
	bool Load(FileParser* sect)override;

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	uint LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:

//	Mix_Music*			music;
//	p2List<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO__