#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "game_entity.h"
#include "paintbrush.h"
#include "spine_data.h"

class SpineEntity : public GameEntity
{
public:
	void draw();
	void init();

	bool flip;

	SpineData spine_data;
};
