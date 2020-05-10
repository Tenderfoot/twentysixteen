#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "common.h"
#include "spine_entity.h"
#include "grid_manager.h"

typedef enum
{
	GRID_IDLE,
	GRID_MOVING,
	GRID_ENDTURN,
	GRID_ATTACKING,
	GRID_DYING,
	GRID_DEAD
}GridCharacterState;

class GridCharacter : public SpineEntity
{
public:

	GridCharacter()
	{
		type = GRID_CHARACTER;
		abilities.push_back(0);
		abilities.push_back(1);
		active_ability = 0;
		visible = true;
	}

	std::map<boundinput, bool> keydown_map;
	GridManager *grid_manager;
	t_vertex desired_pos;
	GridCharacterState state;
	float camera_x_rotation;
	t_vertex draw_position;
	bool dirty_visibiltiy;
	
	std::vector<int> abilities;

	int active_ability;

	float last_time;

	void attack_target(GridCharacter *target)
	{
		state = GRID_ATTACKING;
		spine_data.start_time = SDL_GetTicks();
		spine_data.animation_name = "cast";
		target->die();
	}

	void die()
	{
		state = GRID_DYING;
		spine_data.start_time = SDL_GetTicks();
		spine_data.animation_name = "die";
	}

	void draw()
	{
		if (state == GRID_IDLE)
		{
			draw_position = position;
		}
		
		if (visible)
		{
			glPushMatrix();
			glTranslatef(draw_position.x * 5, draw_position.y, draw_position.z * 5);
			glRotatef(camera_x_rotation*57.25, 0.0f, 1.0f, 0.0f);
			glScalef(0.006f, 0.006f, 0.006f);
			spine_data.draw();
			glPopMatrix();
		}
	}


	// set draw position as position
	// each frame move speed towards destination position
	// once they are close enough to destination, set position as destination,
	// and destination to next coord.

	void set_moving(t_vertex destination)
	{
		desired_pos = destination;
		state = GRID_MOVING;
		draw_position = position;
	}

	virtual void think()
	{
	}

	virtual void update(float time_delta)
	{
		if (state == GRID_MOVING)
		{
			std::vector<t_tile*> path = grid_manager->find_path(position, desired_pos);

			if (path.size() > 0)
			{
				t_tile *next_stop = path.at(path.size() - 1);

				if (abs(draw_position.x - next_stop->x) > 0.01)
				{
					if (draw_position.x < next_stop->x)
						draw_position.x += 0.002*time_delta;
					else
						draw_position.x -= 0.002*time_delta;
				}

				if (abs(draw_position.z - next_stop->y) > 0.01)
				{
					if (draw_position.z < next_stop->y)
						draw_position.z += 0.002*time_delta;
					else
						draw_position.z -= 0.002*time_delta;
				}

				if (t_vertex(t_vertex(next_stop->x,0,next_stop->y) - draw_position).Magnitude() < 0.025)
				{
					position.x = next_stop->x;
					position.z = next_stop->y;

					draw_position = position;
					dirty_visibiltiy = true;
				}
				
				spine_data.animation_name = "walk_two";
			}
			else
			{
				state = GRID_ENDTURN;
				spine_data.animation_name = "idle";
			}
			last_time = SDL_GetTicks();
		}
		else if (state == GRID_ATTACKING)
		{
			if ((((float)SDL_GetTicks()) - spine_data.start_time)/SPINE_TIMESCALE > spSkeletonData_findAnimation(spine_data.skeletonData, spine_data.animation_name)->duration)
			{
				state = GRID_ENDTURN;
			}
		}
		else if (state == GRID_DYING)
		{
			if ((((float)SDL_GetTicks()) - spine_data.start_time) / SPINE_TIMESCALE > spSkeletonData_findAnimation(spine_data.skeletonData, spine_data.animation_name)->duration)
			{
				state = GRID_DEAD;
			}
		}

		if (state != GRID_DEAD)
		{
			spine_data.update_skeleton(time_delta);
		}
	}
};

class EnemyGridCharacter : public GridCharacter
{
public:

	EnemyGridCharacter()
	{
		type = GRID_ENEMYCHARACTER;
		visible = true;
	}

	int enemy_visible()
	{
		grid_manager->reset_visibility();
		grid_manager->compute_visibility_raycast(draw_position.x, draw_position.z, false);
		for (int i = 0; i < grid_manager->entities->size(); i++)
		{
			Entity *current = grid_manager->entities->at(i);
			if (current->type == GRID_CHARACTER)
			{
				if (grid_manager->tile_map[current->position.x][current->position.z].visible && ((GridCharacter*)current)->state != GRID_DEAD)
				{
					return i;
				}
			}
		}
		return -1;
	}

	void think()
	{
		if (state == GRID_IDLE)
		{
			float x, z;
			int enemy_visible_result = enemy_visible();
			if (enemy_visible_result != -1)
			{
				attack_target((GridCharacter*)grid_manager->entities->at(enemy_visible_result));
			}
			else
			{
				x = draw_position.x + 1;
				z = draw_position.z;
				t_vertex grid_pos = t_vertex(int(x), 0, int(z));
				set_moving(grid_pos);
			}
		}
	}

};