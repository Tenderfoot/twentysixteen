#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include "common.h"
#include "level.h"
#include "render_target.h"
#include "vfxgrass.h"
#include "player_entity.h"
#include "base_user_interface.h"
#include "archer_entity.h"

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
		create_mode_entity = new Entity(t_vertex(0,15,0), t_vertex(5,5,5), t_vertex(1,0,0));
		current_type = 0;
		current_entity = 0;

		entity_stringname[ENTITY] = "Base";
		entity_stringname[GAME_ENTITY] = "Game";
		entity_stringname[GRASS_ENTITY] = "Grass";
		entity_stringname[PLAYER_ENTITY] = "Player";
		entity_stringname[EMITTER_ENTITY] = "Emitter";
		entity_stringname[SKELETON_ENTITY] = "Skeleton";
		entity_stringname[ARCHER_ENTITY] = "Archer";
		entity_stringname[ARROW_ENTITY] = "Arrow";

	}
	void take_input(boundinput input, bool type);
	void input_edit(boundinput input, bool type);
	void input_create(boundinput input, bool type);
	void update();
	void build_ui();
	void draw();
	void build_entity();
	void reset_entities();

	// Editor UI stuff
	BaseUserInterface editor_interface;
	std::map<entity_types, std::string> entity_stringname;

	t_vertex get_vertex_from_buffer(std::ifstream *in);

	// file IO to save entity information
	std::string level_name;
	void read_level(std::string level_name);
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