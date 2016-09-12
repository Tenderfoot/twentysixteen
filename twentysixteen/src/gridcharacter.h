#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "spine_entity.h"

class GridCharacter : public SpineEntity
{
public:
	std::map<boundinput, bool> keydown_map;

	void draw()
	{
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
			glScalef(0.006f, 0.006f, 0.006f);
		spine_data.draw();
		glPopMatrix();
	}
};

