#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../spine_data.h"

class LightTechDemo : public BaseTechDemo
{
public:
	LightTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void reset();

	float rotation;
	ModelData level_static;

	// SOIL test
	SpineData spineboy;

};