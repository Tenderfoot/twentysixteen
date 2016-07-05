#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "game_entity.h"
#include "particles.h"

class EnemySideFireEntity: public GameEntity
{
public:

	EnemySideFireEntity()
	{
		type = type = ENEMY_SIDEFIRE_ENTITY;
	}

	EnemySideFireEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = ENEMY_SIDEFIRE_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		initial_size = new_size;
		velocity = t_vertex(0, 0, 0);
		texture = NULL;

		fire_emitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), new_pos, t_vertex(5, 5, 5), SIDE_FIRE, 250, true);
		fire_timer = SDL_GetTicks();
	}
	
	void update(float time_delta)
	{
		fire_emitter.update(time_delta);
		if (SDL_GetTicks() - fire_timer > 3500)
		{
			fire_timer = SDL_GetTicks();
			on = !on;

			if (on)
			{
				fire_emitter.change_max_life(250);
			}
			else
			{
				fire_emitter.change_max_life(10);
			}
		}
	}

	void draw()
	{
		fire_emitter.draw();
	}

	float fire_timer;
	ParticleEmitter fire_emitter;

	bool on;

};