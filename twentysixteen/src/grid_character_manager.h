#pragma once

#include "common.h"
#include "gridcharacter.h"
#include "grid_abilities.h"

class GridCharacterManager
{
public:

	GridCharacterManager()
	{
		this->current_character = 0;
	}

	void spawn_characters(std::vector<Entity*> *entities)
	{
		Entity *current_entity = NULL;
		GridCharacter *new_character = NULL;
		for (int i = 0; i < entities->size(); i++)
		{
			current_entity = entities->at(i);
			if (current_entity->type == GRID_SPAWNPOINT)
			{
				new_character = new GridCharacter();
				new_character->spine_data.load_spine_data("everybody");
				spSkeleton_setSkinByName(new_character->spine_data.skeleton, "witch");
				new_character->spine_data.animation_name = "idle";
				new_character->spine_data.looping = true;
				new_character->grid_manager = grid_manager;
				new_character->position = current_entity->position;
				entities->push_back(new_character);
				characters.push_back(new_character);
			}
			if (current_entity->type == GRID_ENEMYSPAWNPOINT)
			{
				new_character = new EnemyGridCharacter();
				new_character->spine_data.load_spine_data("everybody");
				spSkeleton_setSkinByName(new_character->spine_data.skeleton, "mo");
				new_character->spine_data.animation_name = "idle";
				new_character->spine_data.looping = true;
				new_character->grid_manager = grid_manager;
				new_character->position = current_entity->position;
				entities->push_back(new_character);
				characters.push_back(new_character);
			}
		}
	}

	void get_team_vision(entity_types type)
	{
		grid_manager->reset_visibility();
		for (int i = 0; i < characters.size(); i++)
		{
			if(characters.at(i)->type == type && characters.at(i)->state != GRID_DEAD)
				grid_manager->compute_visibility_raycast(characters.at(i)->position.x, characters.at(i)->position.z, characters.at(i)->type == GRID_CHARACTER);

		}
		for (int i = 0; i < characters.size(); i++)
		{
			if (characters.at(i)->type != type)
			{
				characters.at(i)->visible = grid_manager->position_visible(characters.at(i)->position.x, characters.at(i)->position.z);
			}
			else
				characters.at(i)->visible = true;
		}
	}

	void get_vision()
	{
		grid_manager->reset_visibility();
		grid_manager->compute_visibility_raycast(get_current_character()->position.x, get_current_character()->position.z, true);
	}

	void run(t_vertex mouse_in_space, float camera_rotation_x)
	{
		// if turn is over, go to next grid character in entity list;
		GridCharacter *current_char = get_current_character();

		if (current_char->dirty_visibiltiy && current_char->type == GRID_CHARACTER)
		{
			get_team_vision(current_char->type);
			current_char->dirty_visibiltiy = false;
		}

		if (current_char->state == IDLE)
		{
			grid_manager->good_spot = Ability_Manager::check_valid(current_char, mouse_in_space);
		}

		for (int i = 0; i < characters.size(); i++)
		{
			characters.at(i)->camera_x_rotation = camera_rotation_x;
		}
		
		current_char->think();

		if (current_char->state == GRID_ENDTURN)
		{
			current_char->state = GRID_IDLE;
			current_char->spine_data.animation_name = "idle";

			current_character = (current_character + 1) % characters.size();
			current_char = get_current_character();

			while (current_char->state == GRID_DEAD)
			{
				current_character = (current_character + 1) % characters.size();
				current_char = get_current_character();
			}

			current_char->state = GRID_IDLE;
			get_team_vision(current_char->type);
		}

		// convert mouse position in space to grid coordinates...
		if (current_char->state != GRID_MOVING)
		{
			grid_manager->convert_mouse_coords(mouse_in_space);
		}
	}

	GridCharacter *get_current_character()
	{
		return characters.at(current_character);
	}

	GridManager *grid_manager;
	std::vector<GridCharacter*> characters;
	int current_character;
};
