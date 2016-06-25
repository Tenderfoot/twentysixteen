
#include "audio_controller.h"

std::map<std::string, Mix_Chunk*> AudioController::audio_db = {};

void AudioController::init()
{
	printf("initializing sound..\n");

	int audio_rate = 22050;
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_buffers = 1024;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		exit(1);
	}

	Mix_AllocateChannels(16);
}

void AudioController::play_sound(std::string filename)
{
	Mix_PlayChannel(-1, get_sound(filename), 0);
}

Mix_Chunk* AudioController::get_sound(std::string audio_id)
{
	std::map<std::string, Mix_Chunk*>::iterator it;

	it = audio_db.find(audio_id);

	if (it == audio_db.end())
	{
		audio_db.insert({ audio_id , Mix_LoadWAV(audio_id.c_str()) });
	}

	return audio_db[audio_id];
}