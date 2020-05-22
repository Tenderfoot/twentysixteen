
#include "techdemos/fow_character.h"
#include "fow_building.h"
#include "fow_player.h"
#include "base_user_interface.h"
#include "audio_controller.h"

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

void FOWPlayer::draw_selections()
{
	int i;
	t_vertex draw_position;
	int draw_size;

	for (i = 0; i < entities->size(); i++)
	{
		Entity *current_entity = entities->at(i);

		glPushMatrix();
		if (is_selectable(entities->at(i)->type))
		{
			if (current_entity->type == FOW_CHARACTER || current_entity->type == FOW_GATHERER)
			{
				FOWCharacter *fow_character = (FOWCharacter*)current_entity;
				draw_position = fow_character->draw_position;
			}
			if (current_entity->type == FOW_BUILDING || current_entity->type == FOW_TOWNHALL || current_entity->type == FOW_GOLDMINE)
			{
				draw_position = current_entity->position;
			}

			if (((FOWSelectable*)current_entity)->selected)
			{
				glColor3f(0.5f, 1.0f, 0.5f);
				glDisable(GL_TEXTURE_2D);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
				glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) - 2.5);
				glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) + 2.5);
				glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) - 2.5);
				glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) - 2.5);
				glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) + 2.5);
				glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) + 2.5);
				glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) - 2.5);
				glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) + 2.5);
				glEnd();
				glColor3f(1.0f, 1.0f, 1.0f);
				glEnable(GL_TEXTURE_2D);
			}
		}
		glPopMatrix();
	}
}

void FOWPlayer::draw()
{
	draw_selections();

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

void FOWPlayer::get_selection(t_vertex tile_space, t_vertex tile_end)
{
	t_vertex maxes = t_vertex(std::max(tile_space.x, tile_end.x), std::max(tile_space.z, tile_end.z), 0.0f);
	t_vertex mins = t_vertex(std::min(tile_space.x, tile_end.x), std::min(tile_space.z, tile_end.z), 0.0f);

	// clear selected characters
	if (selection_group.size() > 0)
	{
		for (int i = 0; i < selection_group.size(); i++)
		{
			selection_group.at(i)->clear_selection();
		}
	}

	selection_group.clear();

	// if the box is valid, make a new selection group
	if (int(mins.x) > 0 && int(mins.x) < grid_manager->width)
		if (int(mins.y) > 0 && int(mins.y) < grid_manager->height)
			if (int(maxes.x) > 0 && int(maxes.x) < grid_manager->width)
				if (int(maxes.y) > 0 && int(maxes.y) < grid_manager->height)
				{
					for (int i = 0; i < entities->size(); i++)
					{
						Entity *test = entities->at(i);
						if (is_selectable(test->type))
						{
							if (test->position.x >= mins.x && test->position.z >= mins.y
								&& test->position.x <= maxes.x && test->position.z <= maxes.y)
							{
								selection_group.push_back((FOWSelectable*)test);
								((FOWSelectable*)test)->selected = true;
							}
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
				{
					if (type == true && SDL_GetTicks() - last_poor_warning > 2500)
					{
						AudioController::play_sound("data/sounds/notenough.wav");
						last_poor_warning = SDL_GetTicks();
					}
				}
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