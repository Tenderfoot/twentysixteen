#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../linear_algebra.h"
#include "../grid_manager.h"

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

	//
	GridManager grid_manager;
	int x, y;
	t_3dModel *mymodel;
	SpineData spineboy;
	float camera_rotation_x, camera_rotation_y;
	
};
