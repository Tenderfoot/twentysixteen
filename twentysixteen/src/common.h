#pragma once

// some globals
#define USE_FULLSCREEN 1
#define res_width  2560
#define res_height  1440
#define NUM_TOTAL_ENTITIES 12
#define NUM_PARTICLE_TYPES 4
#define SPINE_TIMESCALE 550

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
	TECHDEMO_BREAKOUT,
	TECHDEMO_JOKE,
	TECHDEMO_FOGOFWAR,
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
	HAT_CLEAR,	// for joysticks
	R_SHOULDER,
	LMOUSE,
	RMOUSE,
	MWHEELDOWN,
	MWHEELUP,
	MIDDLEMOUSE,
	MOUSEMOTION
};

enum particle_types
{
	STAR,
	FIRE,
	POOF,
	SIDE_FIRE
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
	SKELETON_ENTITY,
	ARCHER_ENTITY,
	ARROW_ENTITY,
	BUTTON_ENTITY,
	PORTCULLIS_ENTITY,
	SWORDSMAN_ENTITY,
	ENEMY_SIDEFIRE_ENTITY,
	GRID_CHARACTER,
	GRID_ENEMYCHARACTER,
	GRID_SPAWNPOINT,
	GRID_ENEMYSPAWNPOINT,
};

typedef enum
{
	ANY_SQUARE,
	LINE_OF_SIGHT,
	PATHABLE,
	LINE_OF_SIGHT_TARGET
}t_targetcondition;

typedef enum
{
	MOVE,
	ATTACK
}t_ability_enum;

enum player_states
{
	IDLE,
	WALK_LEFT,
	WALK_RIGHT,
	JUMPING,
	CASTING,
	DEAD
};

enum EDITOR_MODES
{
	CREATE_MODE,
	EDIT_MODE,
	PLAY_MODE
};

static const int war2_autotile_map[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, 13, 13, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, 13, -1, -1,
										13, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, 7, 6, -1, -1,
										7, 6, 5, 5, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, 9, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, 7, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										7, -1, -1, -1, 7, 6, 5, 5, -1, -1,
										-1, 14, -1, -1, -1, 14, -1, -1, -1, -1,
										-1, -1, -1, 12, -1, -1, -1, 6, -1, -1,
										-1, 6, -1, -1, -1, -1, -1, -1, -1, 12,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, 11, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, 6, -1, -1,
										-1, 4, -1, -1, -1, 14, -1, -1, 14, -1,
										-1, -1, -1, -1, -1, -1, -1, 12, -1, -1,
										-1, -1, -1, -1, -1, 6, -1, -1, -1, -1,
										-1, -1, -1, 12, 11, 11, -1, 10, 9, 9,
										-1, 10, -1, -1, -1, -1, -1, -1, 9, 8,
										11, 11, -1, 10, -1, -1, -1, 10, 3, 3,
										-1, 2, 3, -1, 1, 0 };