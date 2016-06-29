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
		color = t_vertex(1.0f, 1.0f, 1.0f);
		size = t_vertex(1, 0.5, 1);
		texture = Paintbrush::get_texture("data/images/arrow.png", false, true);
	}

	void update(float time_delta);
	bool loosed;

};