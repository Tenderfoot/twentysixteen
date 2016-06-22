#pragma once

#include "common.h"
#include "level.h"
#include "render_target.h"
#include "vfxgrass.h"
#include "player_entity.h"

// LevelEditor
// The level editor will have a pointer
// to a basegamelevel's entity list, and will
// manipulate it. This will allow it to create
// and place new entities.

// create mode will let you place an entity of a specific type
// edit mode will be used to modify the entity's attributes

class LevelEditor
{
public:

	void take_input(boundinput input, bool type);
	void update();

	// file IO to save entity information
	void read_level();
	void write_level();

	// The BaseGameLevel's entity list
	std::vector<Entity*> *entities;

	// camera position
	t_vertex camera_position;

	// entity currently modifying
	int current_entity;
};