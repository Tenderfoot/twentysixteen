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
		size.x += 100;
		fire_emitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), new_pos, t_vertex(5, 5, 5), SIDE_FIRE, 250, true);
		fire_timer = SDL_GetTicks();
	}
	
	void update(float time_delta)
	{
		fire_emitter.update(time_delta);
		if (SDL_GetTicks() - fire_timer > 6000)
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

		int j;
		if (on)
		{
			for (j = 0; j < game_entities->size(); j++)
			{
				if (game_entities->at(j)->type == PLAYER_ENTITY)
				{
					if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
					{
						((PlayerEntity*)game_entities->at(j))->catmode = false;
						((PlayerEntity*)game_entities->at(j))->die();
					}
				}

				if (game_entities->at(j)->type == SWORDSMAN_ENTITY)
				{
					if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
					{
						((NPCEntity*)game_entities->at(j))->die();
					}
				}
			}
		}

	}

	void draw()
	{
		glPushMatrix();
			glTranslatef(0.0f, -2.5f, 0.0f);
			fire_emitter.draw();
			glPopMatrix();
		t_collisiongroup test_group;
		test_group.collision_groups.push_back(return_polygon());
		Paintbrush::draw_collision_group(test_group, 0);
	}

	float fire_timer;
	ParticleEmitter fire_emitter;

	bool on;

};