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

	LevelEditor()
	{
		create_mode_entity = new Entity(t_vertex(0,0,0), t_vertex(5,5,5), t_vertex(1,0,0));
		current_type = 0;
	}
	void take_input(boundinput input, bool type);
	void input_edit(boundinput input, bool type);
	void input_create(boundinput input, bool type);
	void update();
	void draw();
	void build_entity();

	// file IO to save entity information
	void read_level();
	void write_level();
	
	// create or edit mode?
	EDITOR_MODES editor_mode;
	Entity *create_mode_entity;
	int current_type;

	// The BaseGameLevel's entity list
	std::vector<Entity*> *entities;
	std::vector<render_target> *render_targets;

	// camera position
	t_vertex camera_position;

	// entity currently modifying
	int current_entity;
};