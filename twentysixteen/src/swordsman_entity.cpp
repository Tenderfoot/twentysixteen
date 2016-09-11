
#include "swordsman_entity.h"
#include "player_entity.h"

void SwordsmanEntity::update(float time_delta)
{
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;

	if (!dead)
	{
		int j;
		for (j = 0; j < game_entities->size(); j++)
		{
			if (game_entities->at(j)->type == PLAYER_ENTITY)
			{
				player_pos = game_entities->at(j)->position;
			}
		}

		if (position.x < player_pos.x + 30 && position.x > player_pos.x - 30)
		{
			spine_data.animation_name = "knight_run";

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
						((PlayerEntity*)game_entities->at(j))->catmode = false;
						((PlayerEntity*)game_entities->at(j))->die();
					}
				}
			}
		}
		else
		{
			velocity = t_vertex(0, 0, 0);
			spine_data.animation_name = "idle";
		}
	}
	else
	{
		velocity.x = 0;
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