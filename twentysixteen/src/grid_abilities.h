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

	t_ability_enum identifier;
	int damage;
	int minrange;
	int maxrange;
	t_targetcondition target_condition;
};

class GridAbilities
{
public:

	GridAbilities()
	{
		ability_db[MOVE] = Ability(MOVE, 0, 1, 100, ANY_SQUARE);
		ability_db[ATTACK] = Ability(ATTACK, 0, 1, 1, ANY_SQUARE);
	}

	static std::map<t_ability_enum, Ability> ability_db;
	void init();
};