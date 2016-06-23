#pragma once

// some globals
#define res_width  1920		
#define res_height  1080

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
	TECHDEMO_POSTPROCESS,
	TECHDEMO_ENEMY,
	TECHDEMO_EDITOR,
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
	BACK,
	NEXT,
	PREVIOUS,
	EDITOR_SAVE,
	EDITOR_CREATE_MODE,
	EDITOR_EDIT_MODE,
	EDITOR_PLAY_MODE,
	EDITOR_DELETE
};

enum rendertarget_types
{
	TYPE_ENTITY,
	TYPE_FACE
};

enum light_types
{
	POINT_LIGHT
};

// Entity - a textured QUAD
// Game_Entity - a quad with physics
enum entity_types
{
	ENTITY,
	GAME_ENTITY,
	GRASS_ENTITY,
	PLAYER_ENTITY,
	EMITTER_ENTITY
};

enum EDITOR_MODES
{
	CREATE_MODE,
	EDIT_MODE,
	PLAY_MODE
};

// proper comparison for char* maps
struct cmp_str
{
	bool operator()(char *a, char *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
