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
#include "entity.h"
#include <map>

class Level
{
public:
	virtual void init() = 0;
	virtual void run(float time_delta) = 0;
	virtual void draw() = 0;
	virtual void draw_hud()
	{
	}
	virtual void reset() = 0;

	// the input, and whether it was pressed or released
	virtual void take_input(boundinput input, bool type) = 0;
	std::map<boundinput, bool> keydown_map;
	
	// Exit level
	levels exit_level = LEVEL_NONE;
	float mousex, mousey;
	bool initialized;

	// mouse information
	t_vertex mouse_relative;
	t_vertex mouse_in_space;

	// this is used to front to back sort entities with respect to the camera
	static t_vertex camera_position;
	static t_vertex camera_lookat;

	struct by_depth_rendertarget {
		bool operator()(render_target left, render_target right) {
			return left.position.z < right.position.z;
		}
	};

	struct by_depth_entity {
		bool operator()(Entity *left, Entity *right) {

			// so the idea is we make a line, and that line is the camera vector
			// we can get this by subtracting the cameras lookat from its position...
			// then project the entities onto that line, and compare their... projected score

			t_vertex projection_vector = camera_lookat - camera_position;

			float left_constant = (projection_vector.DotProduct(left->position) / projection_vector.DotProduct(projection_vector));
			float right_constant = (projection_vector.DotProduct(right->position) / projection_vector.DotProduct(projection_vector));
			t_vertex new_left = projection_vector*left_constant;
			t_vertex new_right = projection_vector*right_constant;
			float left_mag = t_vertex(camera_position-new_left).Magnitude();
			float right_mag = t_vertex(camera_position - new_right).Magnitude();

			return left_mag > right_mag;
		}
	};
};