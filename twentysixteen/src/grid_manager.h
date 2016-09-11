#pragma once

#include "linear_algebra.h"
#include "game_entity.h"

typedef struct
{
	int x, y;
}t_coords;

typedef struct
{
	int x;
	int y;
	int wall;
	std::vector<std::pair<int, int>> visible_tiles;
	bool visible;

	// for pathfinding
	float gscore, fscore;
	t_coords cameFrom;
	bool in_path;

}t_tile;

typedef struct
{
	int dx, dy, D;
	int x, y;
	int x1, y1;
	int octant;
	int xStep, yStep;
	int error;

	void init(int x0, int y0, int x1, int y1)
	{
		dx = abs(x1 - x0);
		dy = abs(y1 - y0);
		xStep = (x1 >= x0) ? 1 : -1;
		yStep = (y1 >= y0) ? 1 : -1;

		error = dx - dy;

		D = dy - dx;
		x = x0;
		y = y0;
		this->x1 = x1;
		this->y1 = y1;
	}

	t_vertex get_point()
	{
		return t_vertex(x, y, 0);
	}

	bool has_next()
	{
		return !(x == x1 && y == y1);
	}

	void next()
	{
		int twoError = 2 * error;

		if (twoError > (-1 * dy)) {
			error -= dy;
			x += xStep;
		}
		if (twoError < dx) {
			error += dx;
			y += yStep;
		}
	}

}t_raycast;

class GridManager
{
public:
	// normal stuff
	void init(int w, int h);

	// for debugging FOV
	void draw_2d();
	void draw_3d();

	// special stuff
	t_polygon get_vision_rect(int i, int j, int i2, int j2);
	bool check_collision(t_polygon rect, int i, int j);
	void compute_visibility(int i, int j);
	void compute_visibility_raycast(int i, int j);
	bool point_can_be_seen(int i, int j, int i2, int j2);
	void set_mouse_coords(int mx, int my);

	// pathfinding stuff
	bool find_path(t_tile *start, t_tile *goal);
	void clear_path();
	t_tile *last_path;

	// loading in tile types
	t_3dModel *tile;
	t_3dModel *wall;

	// other stuff
	int width, height;
	int x, y;
	std::map<int, std::map<int, t_tile>> tile_map;
	int mouse_x, mouse_y;
	bool lookmode;
};