
#include "arrow.h"
#include "player_entity.h"

void ArrowEntity::update(float time_delta)
{
	if (loosed)
		position.x -= (0.025)*time_delta;

	int j;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
			{
				if (((PlayerEntity*)game_entities->at(j))->state != DEAD)
				{
					((PlayerEntity*)game_entities->at(j))->state = DEAD;
					((PlayerEntity*)game_entities->at(j))->spine_data.start_time = SDL_GetTicks();
				}
			}
		}
	}
}
