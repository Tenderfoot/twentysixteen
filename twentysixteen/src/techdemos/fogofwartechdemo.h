#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../linear_algebra.h"

typedef struct
{
	int wall;
	std::vector<std::pair<int, int>> visible_tiles;
	bool visible;
}t_tile;

class FogOfWarTechDemo : public BaseTechDemo
{
public:
	FogOfWarTechDemo()
	{
		this->techdemo_title = "Spine Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void compute_visibility(int i, int j);

	int x, y;

	std::map<int, std::map<int, t_tile>> tile_map;


	t_polygon get_vision_rect(int i, int j, float i2, float j2);

	bool check_collision(t_polygon rect, int i, int j);

	// SOIL test
	SpineData spineboy;
};
