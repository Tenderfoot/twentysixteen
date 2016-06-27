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

class SkeletonEntity : public NPCEntity
{
public:
	SkeletonEntity()
	{
		type = SKELETON_ENTITY;
	}

};
