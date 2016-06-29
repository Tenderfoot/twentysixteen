#pragma once

// some globals
#define res_width  1024		
#define res_height  768

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
	TECHDEMO_GRAVEYARD,
	TECHDEMO_MEADOW,
	TECHDEMO_MARSH,
	TECHDEMO_TOWN,
	TECHDEMO_DUNGEON,
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
	EDITOR_DELETE,
	QUICKSWITCH,	// Q
	USE,			// E
	EDITOR_SCALE_X_PLUS,
	EDITOR_SCALE_X_MINUS,
	EDITOR_SCALE_Y_PLUS,
	EDITOR_SCALE_Y_MINUS,
	EDITOR_SCALE_Z_PLUS,
	EDITOR_SCALE_Z_MINUS,
	EDITOR_RED,
	EDITOR_GREEN,
	EDITOR_BLUE,
	EDITOR_T,
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
	EMITTER_ENTITY,
	SKELETON_ENTITY
};

enum player_states
{
	IDLE,
	WALK_LEFT,
	WALK_RIGHT,
	JUMPING,
	CASTING
};

enum EDITOR_MODES
{
	CREATE_MODE,
	EDIT_MODE,
	PLAY_MODE
};