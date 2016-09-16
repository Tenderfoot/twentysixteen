#pragma once

/* Class SpineEntity */
// This is a GameEntity with an attaches spine character.
// Could be an NPC, the Player, or an enemy.

#include "spine_entity.h"
#include "grid_manager.h"
#include "grid_abilities.h"

typedef enum
{
	GRID_IDLE,
	GRID_MOVING,
	GRID_ENDTURN,
	GRID_ATTACKING
}GridCharacterState;

class GridCharacter : public SpineEntity
{
public:

	GridCharacter()
	{
		type = GRID_CHARACTER;
		abilities.push_back(GridAbilities::ability_db[MOVE]);
		abilities.push_back(GridAbilities::ability_db[ATTACK]);
	}

	std::map<boundinput, bool> keydown_map;
	GridManager *grid_manager;
	t_vertex desired_pos;
	GridCharacterState state;
	float camera_x_rotation;
	t_vertex draw_position;
	std::vector<Ability> abilities;

	float last_time;

	void attack_target(GridCharacter *target)
	{
		state = GRID_ATTACKING;
		spine_data.start_time = SDL_GetTicks();
		spine_data.animation_name = "cast";
	}

	void draw()
	{
		if (state == GRID_IDLE)
		{
			draw_position = position;
		}
		
		glPushMatrix();
			glTranslatef(draw_position.x*5, draw_position.y, draw_position.z*5);
			glRotatef(camera_x_rotation*57.25, 0.0f, 1.0f, 0.0f);
			glScalef(0.006f, 0.006f, 0.006f);
			spine_data.draw();
		glPopMatrix();
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

	void update(float time_delta)
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

					grid_manager->compute_visibility_raycast(position.x, position.z);
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

		spine_data.update_skeleton(time_delta);
	}
			
};

