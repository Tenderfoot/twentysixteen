#pragma once

#include "linear_algebra.h"
class GreenBox;

class FOWSelectable;

class FOWPlayer
{
public:

	FOWPlayer();

	// FOWPlayer for this one
	void get_selection(t_vertex tile_space, t_vertex tile_end)
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

	void take_input(boundinput input, bool type);

	bool is_selectable(entity_types type)
	{
		return (type == FOW_CHARACTER || type == FOW_GATHERER || type == FOW_BUILDING || type == FOW_TOWNHALL || type == FOW_GOLDMINE);
	}

	void draw();
	void update();

	int gold;

	t_vertex gridstart_ui;
	t_vertex gridstart_world;
	t_vertex mouse_in_space;
	t_vertex camera_pos;
	float camera_distance;
	std::vector<FOWSelectable*> selection_group;
	GreenBox *green_box;

	float *mousex;
	float *mousey;

	FOWSelectable *selection;

	std::vector<Entity*> *entities;
	GridManager *grid_manager;

	bool queue_add_toggle;

};