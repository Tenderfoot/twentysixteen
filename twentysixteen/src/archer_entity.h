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
#include "npc_entity.h"
#include "model_data.h"
#include "arrow.h"

class ArcherEntity : public NPCEntity
{
public:
	std::map<boundinput, bool> keydown_map;

	bool turn;

	ArcherEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = ARCHER_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		velocity = t_vertex(0, 0, 0);
	}

	ArrowEntity *current_arrow;
	t_vertex player_pos;

	void init();

	virtual void player_update(float time_delta);
	virtual void update(float time_delta);
};
