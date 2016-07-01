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
#include "particles.h"

class PlayerEntity : public SpineEntity
{
public:

	PlayerEntity()
	{
		type = PLAYER_ENTITY;
		state = IDLE;
	}

	PlayerEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = PLAYER_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		velocity = t_vertex(0, 0, 0);
		state = IDLE;
		spine_data.looping = true;
		staff_emitter = NULL;
	}

	player_states state;

	ParticleEmitter *staff_emitter;

	void handle_keypress(boundinput input, bool type);
	void correct_against_collisiongroup(t_collisiongroup collision_group, float time_delta);
	std::map<boundinput, bool> keydown_map;

	void player_update(float time_delta);
	void update(float time_delta);
	void state_machine();
};
