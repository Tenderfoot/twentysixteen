#pragma once

/* Class Level */
// So this I'm using level in the Unreal Engine 4 sense. That is,
// the title menu, the gameplay and anything else will all be processed / drawn
// by their "level" class. For Example:
//	-Level (abstract)
//		-Title Menu
//		-BaseGameLevel
//			-Each "Scene" in the game
//
//	the main loop has one level active at a time, and thats the level being run and drawn, and taking input.
//	a level can then switch to another level by setting its "openlevel" variable, which will tell the main loop
//	to switch to that level.
//
// The base level class will also handle taking raw input, and translating it valid key events
// for example taking controller inputs or keyboard inputs, and sending them to the base levels as the same input.

#include "common.h"
#include "render_target.h"
#include <map>

class Level
{
public:
	virtual void init() = 0;
	virtual void run(float time_delta) = 0;
	virtual void draw() = 0;
	virtual void reset() = 0;

	// the input, and whether it was pressed or released
	virtual void take_input(boundinput input, bool type) = 0;
	std::map<boundinput, bool> keydown_map;
	
	// Exit level
	levels exit_level = LEVEL_NONE;
	float mousex, mousey;
	bool initialized;

	t_vertex mouse_relative;
	t_vertex mouse_in_space;

	struct by_depth_rendertarget {
		bool operator()(render_target left, render_target right) {
			return left.position.z < right.position.z;
		}
	};
};