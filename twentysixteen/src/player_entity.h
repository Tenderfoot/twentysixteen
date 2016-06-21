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
	void handle_keypress(boundinput input, bool type);
	std::map<boundinput, bool> keydown_map;

	void player_update(float time_delta);
	void update(float delta_time);
};
