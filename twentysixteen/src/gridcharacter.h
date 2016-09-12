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
	t_vertex desired_pos;
	GridCharacterState state;

	float last_time;

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
		if (state == GRID_MOVING)
		{
			std::vector<t_tile*> path = grid_manager->find_path(position, desired_pos);
			if (SDL_GetTicks() - last_time > 500)
			{
				if (path.size() > 0)
				{
					position.x = path.at(path.size() - 1)->x;
					position.z = path.at(path.size() - 1)->y;
					grid_manager->compute_visibility(position.x, position.z);
				}
				else
				{
					state = GRID_IDLE;
				}
				last_time = SDL_GetTicks();
			}
		}
		spine_data.update_skeleton(time_delta);
	}
};

