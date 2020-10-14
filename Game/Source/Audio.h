#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Save module content
	bool Save(pugi::xml_node&) override;

	//Load module content
	bool Load(pugi::xml_node&) override;

	// Control the volume
	void VolumeControl();

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
	
	pugi::xml_node configAudio;

	int musicVolume = 0;
	int fxVolume = 0;

};

#endif // __AUDIO_H__