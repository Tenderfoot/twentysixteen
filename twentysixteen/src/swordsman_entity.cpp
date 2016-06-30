
#include "swordsman_entity.h"
#include "player_entity.h"

void SwordsmanEntity::update(float time_delta)
{
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;

	int j;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;
		}
	}

	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;
		}
	}

	if (position.x < player_pos.x)
	{
		if (velocity.x < 0)
		{
			velocity.x += 0.00005*time_delta;
		}
		else
			velocity.x += 0.00001*time_delta;

		spine_data.flip = true;
	}

	if (position.x > player_pos.x)
	{
		if (velocity.x > 0)
		{
			velocity.x -= 0.00005*time_delta;
		}
		else
		velocity.x -= 0.00001*time_delta;
		spine_data.flip = false;
	}

	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;

			if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
			{
				if (((PlayerEntity*)game_entities->at(j))->state != DEAD)
				{
					((PlayerEntity*)game_entities->at(j))->state = DEAD;
					((PlayerEntity*)game_entities->at(j))->spine_data.start_time = SDL_GetTicks();
					if (((PlayerEntity*)game_entities->at(j))->staff_emitter != NULL)
					{
						((PlayerEntity*)game_entities->at(j))->staff_emitter->kill();
					}
				}
			}
		}
	}

}

void SwordsmanEntity::init()
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, "knight2");
	spine_data.animation_name = "knight_run";
	spine_data.start_time = SDL_GetTicks();
}


void SwordsmanEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.001;
	}
}