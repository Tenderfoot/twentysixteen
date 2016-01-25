#pragma once

#pragma once

/* Class Paintbrush */
// Class Paintbrush will act as a wrapper for some
// of the libraries, and a simple way to store and 
// access loaded data.

#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <SOIL/SOIL.h>
#include <gl/GLU.h>
#include <map>
#include <SDL_mixer.h>

#include "common.h"

// AudioController
//
// Like paintbrush, AudioController is going to be
// a completely static class that allows for playing
// sounds, music, as well as storing loaded audio files.
// It uses SDL_Mixer, which is an extension of base
// SDL_Audio functionality.

class AudioController
{
public:

	static std::map<char*, Mix_Chunk*, cmp_str> audio_db;

	static void init();
	static void play_sound(char *filename);

	static Mix_Chunk* get_sound(char* audio_id);

};

