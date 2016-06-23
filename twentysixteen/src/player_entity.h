#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "spine_entity.h"
#include "paintbrush.h"
#include "spine_data.h"

class PlayerEntity : public SpineEntity
{
public:

	PlayerEntity()
	{
		type = PLAYER_ENTITY;
	}

	PlayerEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = PLAYER_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		velocity = t_vertex(0, 0, 0);
	}

	void handle_keypress(boundinput input, bool type);
	std::map<boundinput, bool> keydown_map;

	void player_update(float time_delta);
	void update(float delta_time);
};
