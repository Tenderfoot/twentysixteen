#pragma once

#include "linear_algebra.h"
class GreenBox;

class FOWSelectable;

class FOWPlayer
{
public:

	FOWPlayer();

	// FOWPlayer for this one
	void get_selection(t_vertex tile_space, t_vertex tile_end);
	virtual void take_input(boundinput input, bool type);

	bool is_selectable(entity_types type)
	{
		return (type == FOW_CHARACTER || type == FOW_GATHERER || type == FOW_BUILDING || type == FOW_TOWNHALL || type == FOW_GOLDMINE);
	}

	void draw();
	void update();
	void draw_selections();

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

	float last_poor_warning;

	FOWSelectable *selection;

	std::vector<Entity*> *entities;
	GridManager *grid_manager;

	bool queue_add_toggle;

};