#pragma once

#include "linear_algebra.h"
#include "game_entity.h"

typedef struct
{
	int wall;
	std::vector<std::pair<int, int>> visible_tiles;
	bool visible;
}t_tile;

class GridManager
{
public:
	// normal stuff
	void init();

	// for debugging FOV
	void draw_2d();

	// special stuff
	t_polygon get_vision_rect(int i, int j, float i2, float j2);
	bool check_collision(t_polygon rect, int i, int j);
	void compute_visibility(int i, int j);

	// other stuff
	int x, y;
	std::map<int, std::map<int, t_tile>> tile_map;
};