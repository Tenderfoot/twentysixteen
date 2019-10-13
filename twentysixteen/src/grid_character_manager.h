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

	void run(t_vertex mouse_in_space, float camera_rotation_x)
	{
		// if turn is over, go to next grid character in entity list;
		GridCharacter *current_char = get_current_character();

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
			grid_manager->compute_visibility_raycast(current_char->position.x, current_char->position.z);
		}

		// convert mouse position in space to grid coordinates...
		if (current_char->state != GRID_MOVING)
		{
			float x, y;
			x = mouse_in_space.x + 2.5;
			y = mouse_in_space.z + 2.5;

			x /= 5;
			y /= 5;

			grid_manager->set_mouse_coords(int(x), int(y));
			if (Ability_Manager::ability_db[(t_ability_enum)current_char->active_ability].target_condition == PATHABLE)
			{
				grid_manager->draw_path(current_char->position);
			}
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
