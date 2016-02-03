#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../spine_data.h"

class SceneTechDemo : public BaseTechDemo
{
public:
	SceneTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	float rotation;

	std::map<boundinput, bool> keydown_map;

	float x, y;
	bool flip;

	ModelData level_static;

	// SOIL test
	SpineData spineboy;

};