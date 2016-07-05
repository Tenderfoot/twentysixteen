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

// MARSH
// make like a wood walkway, replace grass with cat tails / water

class MarshTechDemo : public BaseGameLevel
{
public:
	MarshTechDemo()
	{
	}

	void init();
	ParticleEmitter *side_fire;
};