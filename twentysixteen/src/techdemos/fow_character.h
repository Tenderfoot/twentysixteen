#pragma once

#include "../fow_selectable.h"

class FOWCharacter : public FOWSelectable
{
public:

	FOWCharacter()
	{
		type = FOW_CHARACTER;
		visible = true;
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
				glTranslatef(draw_position.x * 5, draw_position.y + 0.5, (draw_position.z * 5) + 1);
				glRotatef(-90, 1.0f, 0.0f, 0.0f);
				if (desired_position.x > draw_position.x)
					glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
				glScalef(0.0125f, 0.0125f, 0.0125f);
				spine_data.draw();
			glPopMatrix();
		}
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

		if (next_command.type == GATHER)
		{
			desired_position = t_vertex(next_command.target->position.x,0, next_command.target->position.z-1);
			state = GRID_MOVING;
			draw_position = position;
		}


		current_command = next_command;

		FOWSelectable::process_command(next_command);
	};

	void give_command(FOWCommand command)
	{
		command_queue.push_back(command);
	}

	virtual void update(float time_delta)
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
				command_queue.erase(command_queue.begin());
			}
		}
		else if (state == GRID_ATTACKING)
		{
			// why in the fuck is this not a "animationfinished" method on SpineEntity
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


class FOWGatherer : public FOWCharacter
{
public:

	FOWGatherer()
	{
		type = FOW_GATHERER;
		has_gold = false;
		target_town_hall = nullptr;
		target_mine = nullptr;
	}

	bool has_gold;
	float collecting_time;

	FOWSelectable *target_mine;
	FOWSelectable *target_town_hall;


	virtual void update(float time_delta)
	{
		if (state == GRID_MOVING)
		{
			std::vector<t_tile*> path = grid_manager->find_path(position, desired_position);

			if (path.size() > 0)
			{
			}
			else
			{
				if (current_command.type == GATHER)
				{
					// if we're gathering and we've reached our destination we're either at a gold mine or a town hall
					if (has_gold == false)
					{
						position = current_command.target->position;
						draw_position = current_command.target->position;
						visible = false;
						state = GRID_COLLECTING;
						spine_data.animation_name = "idle";
						collecting_time = SDL_GetTicks();
					}
					else
					{
						t_vertex new_position = t_vertex(target_town_hall->position.x, 0, target_town_hall->position.z);
						position = new_position;
						draw_position = new_position;
						visible = false;
						state = GRID_COLLECTING;
						spine_data.animation_name = "idle";
						collecting_time = SDL_GetTicks();
					}
				}
			}
		}

		if (state == GRID_COLLECTING)
		{
			// done dropping off or collecting
			visible = true;

			if (has_gold == false)
			{
				has_gold = true;
				t_vertex new_position = t_vertex(current_command.target->position.x - 1, 0, current_command.target->position.z);
				position = new_position;
				draw_position = new_position;

				std::vector<Entity*> townhall_list = grid_manager->get_entities_of_type(FOW_TOWNHALL);

				if (townhall_list.size() > 0)
				{
					state = GRID_MOVING;
					target_mine = (FOWSelectable*)current_command.target;

					// set the new position to be the closes town hall
					int i;
					for (i = 0; i < townhall_list.size(); i++)
					{
						Entity *town_hall = townhall_list.at(i);
						if (town_hall->type == FOW_TOWNHALL)
						{
							desired_position = t_vertex(town_hall->position.x, 0, town_hall->position.z - 1);
							target_town_hall = (FOWSelectable*)townhall_list.at(i);
						}
					}
				}
				else
				{
					state = GRID_IDLE;
					spine_data.animation_name = "idle";
				}
			}
			else
			{
				has_gold = false;
				
				desired_position = t_vertex(current_command.target->position.x, 0, current_command.target->position.z - 1);
				state = GRID_MOVING;
				draw_position = position;
			}
		}

		FOWCharacter::update(time_delta);
	}

};