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
	bool discovered;

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
	void init();
	void load_map(std::string mapname);

	// for debugging FOV
	void draw_2d();
	void draw_3d();
	void draw_autotile();

	// special stuff
	bool check_collision(t_polygon rect, int i, int j);
	void compute_visibility_raycast(int i, int j, bool discover);
	bool position_visible(int x, int z);
	void reset_visibility();
	bool point_can_be_seen(int i, int j, int i2, int j2);
	void set_mouse_coords(int mx, int my);
	int entity_on_position(t_vertex entity_pos);
	void draw_path(t_vertex start_pos);
	int num_path(t_vertex start_pos);

	// pathfinding stuff
	std::vector<t_tile*> find_path(t_vertex start_pos, t_vertex end_pos);
	std::vector<Entity*> *entities;
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
	bool good_spot;
	GLuint autotile_tex;
	GLuint autotile_tex_war2;
	GLuint active_autotile;

	int war2_autotile_map[255];

	/*
	else if (tex_wall == 131 || tex_wall == 198 || tex_wall == 135 || tex_wall == 195)
		tex_wall = 14;
	else if (tex_wall == 14 || tex_wall == 27 || tex_wall == 15 || tex_wall == 30)
		tex_wall = 13;
	else if (tex_wall == 248 || tex_wall == 252 || tex_wall == 249)
		tex_wall = 3;
	else if (tex_wall == 143 || tex_wall == 159 || tex_wall == 207 || tex_wall == 223)
		tex_wall = 12;
	else if (tex_wall == 255)
		tex_wall = 0;
	else if (tex_wall == 62 || tex_wall == 126 || tex_wall == 63 || tex_wall == 127)
		tex_wall = 5;
	else if (tex_wall == 227 || tex_wall == 243 || tex_wall == 231 || tex_wall == 247)
		tex_wall = 10;
	else if (tex_wall == 228 || tex_wall == 78 || tex_wall == 238 || tex_wall == 229)
		tex_wall = 9;
	else if (tex_wall == 57 || tex_wall == 147 || tex_wall == 187 || tex_wall == 61 || tex_wall == 125 || tex_wall == 151 || tex_wall == 215)
		tex_wall = 6;
	else if (tex_wall == 191)
		tex_wall = 4;
	else if (tex_wall == 239)
		tex_wall = 8;
	else if (tex_wall == 254)
		tex_wall = 1;
	else if (tex_wall == 251)
		tex_wall = 2;*/

};