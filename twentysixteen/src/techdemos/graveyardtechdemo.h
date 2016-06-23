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

// Graveyard
// Dark / foggy / night time. Graves, dirt piles, decrative fence

class GraveyardTechDemo : public BaseGameLevel
{
public:
	GraveyardTechDemo()
	{
	}

	void init();
};