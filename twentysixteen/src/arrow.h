#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "game_entity.h"

class ArrowEntity : public GameEntity
{
public:

	ArrowEntity()
	{
		type = ARROW_ENTITY;
		size = t_vertex(2, 0.5, 1);
		texture = Paintbrush::get_texture("data/images/grass.png", false, false);
	}

	void update(float time_delta);
	bool loosed;

};