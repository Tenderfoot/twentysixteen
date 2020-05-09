#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../linear_algebra.h"
#include "../grid_manager.h"
#include "../gridcharacter.h"
#include "../grid_character_manager.h"

class FogOfWarTechDemo : public BaseTechDemo
{
public:
	FogOfWarTechDemo()
	{
		this->techdemo_title = "Fog of War Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void draw_hud();
	void reset();

	// Dungeon stuff
	GridManager grid_manager;
	GridCharacterManager character_manager;
	std::vector<Entity*> entities;

	// UI stuff
	CharacterWidget *char_widget;
	std::vector<std::string> combat_log;
	AbilityBar *ability_bar;

	// other stuff
	int x, y;
	float camera_rotation_x, camera_rotation_y;
	float camera_distance;
	bool lookmode;
	
};
