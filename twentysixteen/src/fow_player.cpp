
#include "techdemos/fow_character.h"
#include "fow_building.h"
#include "fow_player.h"
#include "base_user_interface.h"


FOWPlayer::FOWPlayer()
{
	queue_add_toggle = false;
	gold = 0;
	green_box = new GreenBox();
	camera_distance = 50.0f;
	camera_pos.x = 15;
	camera_pos.z = 10;
}

void FOWPlayer::update()
{
	green_box->width = *mousex;
	green_box->height = *mousey;
}

void FOWPlayer::draw()
{
	if (selection_group.size() == 1)
	{
		Entity *current = selection_group.at(0);
		if (current->type == FOW_GATHERER)
		{
			FOWGatherer *builder = (FOWGatherer*)current;

			if (builder->build_mode)
			{
				FOWBuilding new_building(grid_manager->mouse_x, grid_manager->mouse_y, 3);

				if (grid_manager->space_free(t_vertex(grid_manager->mouse_x, grid_manager->mouse_y, 0.0f), 3))
				{
					new_building.spine_data.color = t_vertex(0.0f, 1.0f, 0.0f);
					builder->good_spot = true;
				}
				else
				{
					new_building.spine_data.color = t_vertex(1.0f, 0.0f, 0.0f);
					builder->good_spot = false;
				}

				new_building.draw();
			}

		}
	}

}

void FOWPlayer::take_input(boundinput input, bool type)
{

	if (input == LMOUSE && type == true)
	{
		green_box->x = *mousex;
		green_box->y = *mousey;
		green_box->mouse_in_space = mouse_in_space;
		green_box->visible = true;
	}

	if (input == LMOUSE && type == false)
	{

		if (selection_group.size() == 1)
		{
			if (selection_group.at(0)->type == FOW_GATHERER)
			{
				FOWGatherer *gatherer = ((FOWGatherer*)selection_group.at(0));
				if (gatherer->build_mode && gatherer->good_spot)
				{
					gatherer->give_command(FOWCommand(BUILD_BUILDING, t_vertex(grid_manager->mouse_x, 0.0f, grid_manager->mouse_y)));
				}
			}
		}

		green_box->visible = false;
		get_selection(grid_manager->convert_mouse_coords(green_box->mouse_in_space), grid_manager->convert_mouse_coords(mouse_in_space));

/*
		if (selection_group.size() > 0)
			char_widget->character = new_player->selection_group.selected_characters.at(0);
		else
			char_widget->character = nullptr;*/
	}

	if (input == RIGHT && type == true)
	{
		camera_pos.x++;
	}

	if (input == LEFT && type == true)
	{
		camera_pos.x--;
	}

	if (input == UP && type == true)
	{
		camera_pos.z--;
	}

	if (input == DOWN && type == true)
	{
		camera_pos.z++;
	}

	if (input == EDITOR_T && type == true)
	{
		grid_manager->use_tex = !grid_manager->use_tex;
	}

	if (input == NEXT && type == true)
	{
		grid_manager->randomize_map();
	}

	if (input == HOTKEY_B && type == true)
	{
		if (selection_group.size() == 1)
		{
			if (selection_group.at(0)->type == FOW_GATHERER)
			{
				((FOWGatherer*)selection_group.at(0))->build_mode = true;
			}
		}
	}

	if (input == USE)
	{
		queue_add_toggle = type;

		if (selection_group.size() == 1)
		{
			if (selection_group.at(0)->type == FOW_TOWNHALL)
			{
				if (gold > 0)
				{
					gold--;
					selection_group.at(0)->process_command(FOWCommand(BUILD_UNIT, FOW_GATHERER));
					FOWGatherer *new_gatherer = new FOWGatherer();
					new_gatherer->spine_data.load_spine_data("everybody");
					spSkeleton_setSkinByName(new_gatherer->spine_data.skeleton, "farm");
					new_gatherer->spine_data.animation_name = "idle";
					new_gatherer->owner = this;
					new_gatherer->spine_data.looping = true;
					new_gatherer->grid_manager = grid_manager;
					new_gatherer->position = t_vertex(selection_group.at(0)->position.x + 4, 0.0f, selection_group.at(0)->position.z);
					entities->push_back(new_gatherer);
				}
				else
					printf("not enough minerals\n");
			}
		}
	}

	if (input == RMOUSE && type == true)
	{
		t_vertex hit_position = grid_manager->convert_mouse_coords(mouse_in_space);
		Entity *hit_target = nullptr;

		// lets see if theres something on the hit position...
		for (int i = 0; i < entities->size(); i++)
		{
			Entity *test = entities->at(i);
			if (is_selectable(test->type))
			{
				if (test->position.x == hit_position.x && test->position.z == hit_position.z
					&& test->position.x == hit_position.x && test->position.z == hit_position.z)
				{
					hit_target = test;
				}
			}
		}

		if (selection_group.size() > 0)
		{
			for (int i = 0; i < selection_group.size(); i++)
			{
				if (selection_group.at(i)->type == FOW_CHARACTER)
				{
					if (queue_add_toggle == false)
						selection_group.at(i)->command_queue.clear();
					((FOWCharacter*)selection_group.at(i))->give_command(FOWCommand(MOVE, t_vertex(hit_position.x + i, 0.0f, hit_position.z + i % 2)));
				}

				if (selection_group.at(i)->type == FOW_GATHERER)
				{
					FOWGatherer *gatherer = (FOWGatherer*)selection_group.at(i);

					if (hit_target != nullptr)
					{
						if (hit_target->type == FOW_GOLDMINE)
						{
							gatherer->give_command(FOWCommand(GATHER, hit_target));
						}
						else if (hit_target->type == FOW_GATHERER)
						{
							gatherer->give_command(FOWCommand(ATTACK, hit_target));
						}
					}
					else
					{
						if (queue_add_toggle == false)
							selection_group.at(i)->command_queue.clear();
						((FOWCharacter*)selection_group.at(i))->give_command(FOWCommand(MOVE, t_vertex(hit_position.x + i, 0.0f, hit_position.z + i % 2)));
					}
				}
			}
		}
	}

	if (input == MWHEELUP)
	{
		if (camera_distance > 5)
			camera_distance--;
	}

	if (input == MWHEELDOWN)
	{
		if (camera_distance < 100)
			camera_distance++;
	}

}