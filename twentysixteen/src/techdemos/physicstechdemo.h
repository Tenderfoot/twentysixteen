#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../game_entity.h"
#include "../spine_data.h"

class PhysicsTechDemo : public BaseTechDemo
{
public:
	PhysicsTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	bool check_collision();


	ModelData level_static;
	t_collisiongroup collision_group;
	SpineData spineboy;

	bool flip;

	GameEntity box;

};