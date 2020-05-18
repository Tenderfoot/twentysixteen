#pragma once

#include "linear_algebra.h"
#include "./techdemos/fow_character.h"
#include "./fow_building.h"

typedef struct
{
	std::vector<FOWSelectable*> selected_characters;
} t_selectiongroup;

class FOWPlayer
{
public:

	FOWPlayer()
	{
		queue_add_toggle = false;
		gold = 0;
	}

	// FOWPlayer for this one
	void get_selection(t_vertex tile_space, t_vertex tile_end)
	{
		t_vertex maxes = t_vertex(std::max(tile_space.x, tile_end.x), std::max(tile_space.z, tile_end.z), 0.0f);
		t_vertex mins = t_vertex(std::min(tile_space.x, tile_end.x), std::min(tile_space.z, tile_end.z), 0.0f);

		// clear selected characters
		if (selection_group.selected_characters.size() > 0)
		{
			for (int i = 0; i < selection_group.selected_characters.size(); i++)
			{
				selection_group.selected_characters.at(i)->clear_selection();
			}
		}

		selection_group.selected_characters.clear();

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
									selection_group.selected_characters.push_back((FOWSelectable*)test);
									((FOWSelectable*)test)->selected = true;
								}
							}
						}
					}
	}

	void draw()
	{
		
		if (selection_group.selected_characters.size() == 1)
		{
			Entity *current = selection_group.selected_characters.at(0);
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

	bool is_selectable(entity_types type)
	{
		return (type == FOW_CHARACTER || type == FOW_GATHERER || type == FOW_BUILDING || type == FOW_TOWNHALL || type == FOW_GOLDMINE);
	}

	int gold;

	t_vertex gridstart_ui;
	t_vertex gridstart_world;
	t_selectiongroup selection_group;

	FOWSelectable *selection;

	std::vector<Entity*> *entities;
	GridManager *grid_manager;

	bool queue_add_toggle;

};