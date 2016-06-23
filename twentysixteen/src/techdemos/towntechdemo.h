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

// TOWN
// Town is like meadow but with buildings

class TownTechDemo : public BaseGameLevel
{
public:
	TownTechDemo()
	{
	}

	void init();
};