#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"

class ModelToPolygonTechDemo : public BaseTechDemo
{
public:
	ModelToPolygonTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	t_3dModel *model_data;

	bool draw_plane;
	bool draw_model;

};