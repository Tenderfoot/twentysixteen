#pragma once

#include <map>
#include "common.h"
#include "gridcharacter.h"

class Ability
{
public:
	Ability()
	{}

	Ability(t_ability_enum identifier, int damage, int minrange, int maxrange, t_targetcondition target_condition)
	{
		this->identifier = identifier;
		this->damage = damage;
		this->minrange = minrange;
		this->maxrange = maxrange;
		this->target_condition = target_condition;
	}

	t_ability_enum get_identifier()
	{
		return identifier;
	}

	t_ability_enum identifier;
	int damage;
	int minrange;
	int maxrange;
	t_targetcondition target_condition;
};

class Ability_Manager
{
public:

	static void build_abilities()
	{
		ability_db[MOVE] = Ability(MOVE, 0, 0, 0, PATHABLE);
		ability_db[ATTACK] = Ability(ATTACK, 1, 1, 1, LINE_OF_SIGHT);
	}

	static Ability get_ability(t_ability_enum index)
	{
		return ability_db[index];
	}

	static void use_ability(GridCharacter *activator, t_vertex activate_position)
	{
		// gather some info
		float x, y;
		x = activate_position.x + 2.5;
		y = activate_position.z + 2.5;
		x /= 5;
		y /= 5;
		t_vertex grid_pos = t_vertex(int(x), 0, int(y));

		// handle abilities
		if (activator->active_ability == MOVE)
		{
			if(activator->state == IDLE)
				activator->set_moving(grid_pos);
		}
		if (activator->active_ability == ATTACK)
		{
			if (grid_manager->entity_on_position(t_vertex(int(x), 0, int(y))) != -1 && activator->state == IDLE)
			{
				Entity *entity_on_pos = grid_manager->entities->at(grid_manager->entity_on_position(t_vertex(int(x), 0, int(y))));
				activator->attack_target((GridCharacter*)entity_on_pos);
			}
		}
	}

	static std::map<t_ability_enum, Ability> ability_db;
	static GridManager *grid_manager;
};