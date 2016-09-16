#pragma once

#include <map>

typedef enum
{
	ANY_SQUARE,
	LINE_OF_SIGHT,
	PATHABLE,
	LINE_OF_SIGHT_TARGET
}t_targetcondition;

typedef enum
{
	MOVE,
	ATTACK
}t_ability_enum;

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