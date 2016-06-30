
#include "swordsman_entity.h"

void SwordsmanEntity::update(float time_delta)
{
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
/*
	int j;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;
		}
	}

	if (position.x < player_pos.x)
	{
		velocity.x -= 0.0005*time_delta;
	}

	if (position.x > player_pos.x)
	{
		velocity.x -= 0.0005*time_delta;
	}
*/
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