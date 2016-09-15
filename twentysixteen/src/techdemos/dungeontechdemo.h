#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../linear_algebra.h"
#include "../grid_manager.h"
#include "../gridcharacter.h"

class DungeonTechDemo : public BaseTechDemo
{
public:
	DungeonTechDemo()
	{
		this->techdemo_title = "Dungeon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void draw_hud();
	void reset();

	// Dungeon stuff
	GridManager grid_manager;
	std::vector<Entity*> entities;
	t_3dModel *mymodel;
	GridCharacter *test; 
	GridCharacter *current_char;
	int current_char_entity;
	CharacterWidget *char_widget;
	std::vector<std::string> combat_log;

	// other stuff
	int x, y;
	float camera_rotation_x, camera_rotation_y;
	float camera_distance;
	bool lookmode;
};
