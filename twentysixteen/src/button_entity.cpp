#include "button_entity.h"

void ButtonEntity::update(float time_delta)
{
	// I'm going to iterate a lot here
	// It'll be fixed sooner than later
	int i,j;
	bool found = false;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY || game_entities->at(j)->type == SWORDSMAN_ENTITY || game_entities->at(j)->type == SKELETON_ENTITY)
		{
			if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
			{
				for (i = 0; i < game_entities->size(); i++)
				{
					if (game_entities->at(i)->type == PORTCULLIS_ENTITY)
					{
						if (game_entities->at(i)->activation_index == this->activation_index)
						{
							((GameEntity*)game_entities->at(i))->activate();
							found = true;
						}
					}
				}
			}
		}
	}

	if (found == false)
	{
		for (i = 0; i < game_entities->size(); i++)
		{
			if (game_entities->at(i)->type == PORTCULLIS_ENTITY)
			{
				if (game_entities->at(i)->activation_index == this->activation_index)
				{
					((GameEntity*)game_entities->at(i))->activated = false;
				}
			}
		}
	}
}