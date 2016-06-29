#include "button_entity.h"

void ButtonEntity::update(float time_delta)
{

	// I'm going to iterate a lot here
	// It'll be fixed sooner than later

	int i,j;

	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
			{
				printf("hit here!");
				for (i = 0; i < game_entities->size(); i++)
				{
					if (game_entities->at(i)->type == PORTCULLIS_ENTITY)
					{
						((GameEntity*)game_entities->at(i))->activate();
					}
				}
			}
		}
	}

}