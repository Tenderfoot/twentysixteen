#pragma once

#include "common.h"
#include "level.h"
#include "render_target.h"
#include "vfxgrass.h"

// LevelEditor
// The level editor will have a pointer
// to a basegamelevel's entity list, and will
// manipulate it. This will allow it to create
// and place new entities.

class LevelEditor
{
public:

	void take_input(boundinput input, bool type);
	void update();

	// The BaseGameLevel's entity list
	std::vector<Entity*> *entities;

	// camera position
	t_vertex camera_position;

	// entity currently modifying
	int current_entity;
};