#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../linear_algebra.h"
#include "../grid_manager.h"

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

	//
	GridManager grid_manager;
	int x, y;
	t_3dModel *mymodel;
	SpineData spineboy;
	float camera_rotation_x, camera_rotation_y;
	float camera_distance;
	bool lookmode;

};
