#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../particles.h"



class EmitterTechDemo : public BaseTechDemo
{
public:
	EmitterTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void reset();

	ModelData level_static;

	ParticleEmitter myemitter;

	float rotation;

};