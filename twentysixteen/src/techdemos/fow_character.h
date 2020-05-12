#pragma once

#include "../gridcharacter.h"


class FOWCharacter : public GridCharacter
{
public:

	FOWCharacter()
	{
		type = FOW_CHARACTER;
		abilities.push_back(0);
		abilities.push_back(1);
		active_ability = 0;
		visible = true;
		selected = false;
	}


	void draw()
	{
		if (state == GRID_IDLE)
		{
			draw_position = position;
		}


			glPushMatrix();
			glTranslatef(draw_position.x * 5, draw_position.y+0.5, (draw_position.z * 5)+1);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			if (desired_pos.x > draw_position.x)
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.0125f, 0.0125f, 0.0125f);
			spine_data.draw();
			glPopMatrix();
		
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

				if (t_vertex(t_vertex(next_stop->x, 0, next_stop->y) - draw_position).Magnitude() < 0.025)
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
				state = GRID_IDLE;
				spine_data.animation_name = "idle";
			}
			last_time = SDL_GetTicks();
		}
		else if (state == GRID_ATTACKING)
		{
			if ((((float)SDL_GetTicks()) - spine_data.start_time) / SPINE_TIMESCALE > spSkeletonData_findAnimation(spine_data.skeletonData, spine_data.animation_name)->duration)
			{
				state = GRID_IDLE;
			}
		}
		else if (state == GRID_DYING)
		{
			if ((((float)SDL_GetTicks()) - spine_data.start_time) / SPINE_TIMESCALE > spSkeletonData_findAnimation(spine_data.skeletonData, spine_data.animation_name)->duration)
			{
				state = GRID_IDLE;
			}
		}

		if (state != GRID_DEAD)
		{
			spine_data.update_skeleton(time_delta);
		}
	}

	bool selected;

};
