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

class PostProcessDemo : public BaseGameLevel
{
public:
	PostProcessDemo()
	{
	}

	void run(float time_delta);
	void init();
	void take_input(boundinput input, bool type);
	void reset();
	void draw();

	GLubyte *data;
	GLuint scene_texture;

	ParticleEmitter myemitter;
	PlayerEntity spineboy;
};