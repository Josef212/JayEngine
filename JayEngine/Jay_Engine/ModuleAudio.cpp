#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "FileParser.h"

#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)//, music(NULL)
{
	_LOG(LOG_STD, "Audio: Creation.");
	name.assign("module_audio");
}

// Destructor
ModuleAudio::~ModuleAudio()
{
	_LOG(LOG_STD, "Audio: Destroying.");
}

// Called before render is available
bool ModuleAudio::Init(FileParser* conf)
{
	_LOG(LOG_STD, "Audio: Init.");
	bool ret = true;
	/*SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}
	*/
	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	_LOG(LOG_STD, "Audio: CleanUp.");
	/*LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;

	for(item = fx.getFirst(); item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);*/
	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	/*
	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);*/
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;
/*
	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}
	*/
	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;
	/*
	Mix_Chunk* chunk = NULL;
	
	if(fx.at(id-1, chunk) == true)
	{
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;
	}
	*/
	return ret;
}

bool ModuleAudio::Save(FileParser* sect)
{
	sect->AddBool("test", true);
	sect->AddInt("teste", 10);
	float a[4] = { 0.f, 1.f, 2.f, 3.f };
	sect->AddFloatArray("test_ar", a, 4);
	
	sect->AddFloat3("test_float3", float3(2, 1, 2));

	return true;
}

bool ModuleAudio::Load(FileParser* sect)
{
	int b = (int)sect->GetBool("test", false);

	_LOG(LOG_WARN, "Getting a bool from audio module: %d.", b);
	_LOG(LOG_WARN, "Getting an int from audio module: %d.", sect->GetInt("teste", 0));

	_LOG(LOG_INFO_REM, "Getting float array: %f.", sect->GetFloat("test_ar", 0.f, 2));

	float3 g = sect->GetFloat3("test_float3", float3(0, 0, 0));
	_LOG(LOG_INFO_REM, "Getting float3.x: %f.", g.x);
	_LOG(LOG_INFO_REM, "Getting float3.y: %f.", g.y);
	_LOG(LOG_INFO_REM, "Getting float3.z: %f.", g.z);

	return true;
}