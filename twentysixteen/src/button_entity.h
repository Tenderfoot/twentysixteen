#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "game_entity.h"

class ButtonEntity : public GameEntity
{
public:
	ButtonEntity()
	{
		type = BUTTON_ENTITY;
		color = t_vertex(1.0f, 1.0f, 1.0f);
		size = t_vertex(4, 2, 1);
		texture = Paintbrush::get_texture("data/images/grass.png", false, true);
	}

	ButtonEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = BUTTON_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		velocity = t_vertex(0, 0, 0);
	}

	void update(float time_delta);
};