#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../particles.h"
#include "../spine_data.h"
#include "../VFXGrass.h"

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
	void reset();

	// Level Entities
	std::vector<Entity*> entities;

	float x, y;
	bool flip;

	ParticleEmitter myemitter;
	ModelData level_static;
	SpineData spineboy;

};