#pragma once

// some globals
#define res_width  1920		
#define res_height  1080.

#include <stdio.h>
#include <string>

enum levels
{
	LEVEL_NONE,
	TECHDEMO_BASE,
	TECHDEMO_SPINE,
	TECHDEMO_TTF,
	TECHDEMO_AUDIO,
	TECHDEMO_MODEL,
	TECHDEMO_SHADER,
	TECHDEMO_MODELTOPOLY,
	TECHDEMO_GRASS,
	TECHDEMO_SCENE,
	TECHDEMO_EMITTER,
	TECHDEMO_PHYSICS,
	TECHDEMO_LIGHTS,
	QUIT
};

enum boundinput
{
	NO_BIND,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ACTION,
	BACK
};

enum light_types
{
	POINT_LIGHT
};

// proper comparison for char* maps
struct cmp_str
{
	bool operator()(char *a, char *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
