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
#include "../npc_entity.h"
#include "../skeleton_entity.h"

// This used to inherit from basetechdemo, but
// it makes more sense to make it inherit from basegamelevel.

class EnemyTechDemo : public BaseGameLevel
{
public:
	
	EnemyTechDemo(std::string level_name)
	{
		init_level(level_name);
	}

	void run(float time_delta);
	void init();
	void reset();

	SkeletonEntity skeleton;
};