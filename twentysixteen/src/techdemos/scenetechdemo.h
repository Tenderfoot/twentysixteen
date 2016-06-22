#pragma once

#include "basetechdemo.h"
#include "../base_gamelevel.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../particles.h"
#include "../spine_data.h"
#include "../VFXGrass.h"
#include "../modelprop_entity.h"
#include "../player_entity.h"

// This used to inherit from basetechdemo, but
// it makes more sense to make it inherit from basegamelevel.

class SceneTechDemo : public BaseGameLevel
{
public:
	SceneTechDemo()
	{
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void reset();

	static t_vertex camera_position;

	ParticleEmitter myemitter;
	PlayerEntity spineboy;
};