#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "spine_entity.h"
#include "grid_manager.h"

typedef enum
{
	GRID_IDLE,
	GRID_MOVING
}GridCharacterState;

class GridCharacter : public SpineEntity
{
public:
	std::map<boundinput, bool> keydown_map;
	GridManager *grid_manager;

	void draw()
	{
		glPushMatrix();
			glTranslatef(position.x*5, position.y, position.z*5);
			glScalef(0.006f, 0.006f, 0.006f);
		spine_data.draw();
		glPopMatrix();
	}

	void update(float time_delta)
	{
		spine_data.update_skeleton(time_delta);
	}
};

