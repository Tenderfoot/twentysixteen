
#include "techdemos/fow_character.h"
#include "fow_building.h"
#include "fow_player.h"

void FOWGatherer::OnReachDestination()
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
			owner->gold++;
			visible = false;
			state = GRID_COLLECTING;
			spine_data.animation_name = "idle";
			collecting_time = SDL_GetTicks();
		}
	}

	if (current_command.type == BUILD_BUILDING)
	{
		FOWTownHall *test = new FOWTownHall(current_command.position.x, current_command.position.z, 3);
		grid_manager->entities->push_back(test);
		FOWCharacter::set_idle();
	}

	FOWCharacter::OnReachDestination();
}

void FOWGatherer::process_command(FOWCommand next_command)
{
	if (next_command.type == GATHER)
	{
		desired_position = t_vertex(next_command.target->position.x, 0, next_command.target->position.z - 1);
		state = GRID_MOVING;
		draw_position = position;
		current_path = grid_manager->find_path(position, desired_position);
	}

	if (next_command.type == BUILD_BUILDING)
	{
		printf("Building Building\n");
		desired_position = next_command.position;
		state = GRID_MOVING;
		draw_position = position;
		current_path = grid_manager->find_path(position, desired_position);
	}

	FOWCharacter::process_command(next_command);
}

void FOWGatherer::PathBlocked()
{
	if (current_command.target != nullptr)
	{
		t_vertex new_position;
		if (has_gold)
			new_position = t_vertex(target_town_hall->position.x + 1, 0, target_town_hall->position.z + 1);
		else
			if (current_command.type == ATTACK)
				new_position = t_vertex(current_command.target->position.x, 0, current_command.target->position.z - 1);
			else
				new_position = t_vertex(current_command.position.x + 1, 0, current_command.position.z + 1);

		desired_position = new_position;
		current_path = grid_manager->find_path(position, desired_position);
		while (current_path.size() == 0)
		{
			new_position = t_vertex(new_position.x + 1, 0, new_position.z);
			desired_position = new_position;
			current_path = grid_manager->find_path(position, new_position);
		}
	}
}

void FOWGatherer::update(float time_delta)
{
	if (state == GRID_COLLECTING)
	{
		// done dropping off or collecting
		if (SDL_GetTicks() - collecting_time > 1000)
		{
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
					current_path = grid_manager->find_path(position, desired_position);
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
				current_path = grid_manager->find_path(position, desired_position);
				state = GRID_MOVING;
				draw_position = position;
			}
		}
	}
	FOWCharacter::update(time_delta);
}