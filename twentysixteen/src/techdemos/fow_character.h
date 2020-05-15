#pragma once

#include "../fow_selectable.h"

class FOWCharacter : public FOWSelectable
{
public:

	FOWCharacter()
	{
		type = FOW_CHARACTER;
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
			if (desired_position.x > draw_position.x)
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			glScalef(0.0125f, 0.0125f, 0.0125f);
			spine_data.draw();
			glPopMatrix();
		
	}

	void process_command(FOWCommand next_command)
	{
		// Every Character can move, buildings can't
		// Some Buildings can attack but thats ok
		if (next_command.type == MOVE)
		{
			desired_position = next_command.position;
			state = GRID_MOVING;
			draw_position = position;
		}

		current_command = next_command;

		FOWSelectable::process_command(next_command);
	};

	void give_command(FOWCommand command)
	{
		command_queue.clear();
		command_queue.push_back(command);
	}

	void update(float time_delta)
	{
		if (state == GRID_MOVING)
		{

			// This is actually disgusting, this shouldn't run every frame, only when it first starts moving, or if it gets stopped
			// currently happening every frame for every character
			std::vector<t_tile*> path = grid_manager->find_path(position, desired_position);

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

					// a new move command came in, process after you hit the next grid space
					if (!(current_command == command_queue.at(0)))
					{
						process_command(command_queue.at(0));
					}
				}

				spine_data.animation_name = "walk_two";
			}
			else
			{
				state = GRID_IDLE;
				spine_data.animation_name = "idle";
			}
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
		else if (state == GRID_IDLE)
		{
			if (command_queue.size() > 0)
				process_command(command_queue.at(0));
		}

		if (state != GRID_DEAD)
		{
			spine_data.update_skeleton(time_delta);
		}
	}

	t_vertex draw_position;
	t_vertex desired_position;
	bool dirty_visibiltiy;
};