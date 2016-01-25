
#include "audio_controller.h"

std::map<char*, Mix_Chunk*, cmp_str> AudioController::audio_db = {};

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

void AudioController::play_sound(char *filename)
{
	char *dir = new char[256];
	sprintf_s(dir, sizeof(char) * 256, "data/sounds/%s.wav", filename);

	Mix_PlayChannel(-1, get_sound(dir), 0);
}

Mix_Chunk* AudioController::get_sound(char* audio_id)
{
	std::map<char*, Mix_Chunk*, cmp_str>::iterator it;

	it = audio_db.find(audio_id);

	if (it == audio_db.end())
	{
		audio_db[audio_id] = Mix_LoadWAV(audio_id);
	}

	return audio_db[audio_id];
}