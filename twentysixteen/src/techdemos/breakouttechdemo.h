#pragma once
#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"
#include "../model_data.h"
#include "../game_entity.h"
#include "../paintbrush.h"
#include "../particles.h"

typedef struct
{
	t_vertex position;
	t_vertex size;
	t_vertex velocity;
}game_rect;

class BreakoutTechDemo : public BaseTechDemo
{
public:
	BreakoutTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	float rotation;
	ModelData level_static;

	// Game stuff
	GameEntity ball;
	GameEntity paddle;
	std::vector<GameEntity> bricks;

	ParticleEmitter myemitter;

	std::vector<Entity*> entities;

	bool start;
	bool move_left, move_right;

	// SOIL test
	SpineData spineboy;

};