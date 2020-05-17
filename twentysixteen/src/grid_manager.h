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
	
	// 0 - dirt
	// 1 - grass
	// 2 - water
	int type; 
	
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
	void randomize_map();

	// for debugging FOV
	void draw_2d();
	void draw_3d();
	void draw_autotile();
	int calculate_tile(int i, int j, int current_type);
	bool check_compatible(int i, int j, int current_type);
	void dropblob(int i, int j, int blobtype);
	void cull_orphans();

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
	t_vertex convert_mouse_coords(t_vertex mouse_space);
	std::vector<Entity*> get_entities_of_type(entity_types type);

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
	bool use_tex;
	GLuint fake_tex[4];
	GLuint real_tex[4];

};