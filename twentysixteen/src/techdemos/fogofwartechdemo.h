#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"

class FogOfWarTechDemo : public BaseTechDemo
{
public:
	FogOfWarTechDemo()
	{
		this->techdemo_title = "Spine Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	std::map<int, std::map<int, int>> tile_map;

	// SOIL test
	SpineData spineboy;
};
