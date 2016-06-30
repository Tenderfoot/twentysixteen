#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "game_entity.h"

class PortcullisEntity : public GameEntity
{
public:

	PortcullisEntity()
	{
		type = type = PORTCULLIS_ENTITY;
	}

	PortcullisEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = PORTCULLIS_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		initial_size = new_size;
		velocity = t_vertex(0, 0, 0);
		texture = Paintbrush::get_texture("data/images/metal_gate.png", false, true);
		returner = true;
	}

	void draw();
	void update(float time_delta);
	void activate();
	void reset();

	bool returner;

};