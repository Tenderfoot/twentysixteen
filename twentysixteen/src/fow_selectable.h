#pragma once

#include "linear_algebra.h"

class FOWCommand
{
public:

	FOWCommand()
	{
	}

	FOWCommand(t_ability_enum type, t_vertex position)
	{
		this->type = type;
		this->position = position;
	}

	FOWCommand(t_ability_enum type, Entity *target)
	{
		this->type = type;
		this->target = target;
	}
	
	FOWCommand(t_ability_enum type, entity_types unit_type)
	{
		this->type = type;
		this->unit_type = unit_type;
	}

	bool operator==(const FOWCommand& rhs)
	{
		return (this->type == rhs.type && this->position.x == rhs.position.x &&  this->position.z == rhs.position.z);
	}

	t_vertex position;
	t_ability_enum type;
	entity_types unit_type;
	Entity *target;
};


class FOWSelectable : public SpineEntity
{
public:

	FOWSelectable()
	{
	}

	virtual void process_command(FOWCommand next_command)
	{
	};

	virtual void clear_selection() 
	{
		selected = false;
	};

	FOWCommand current_command;

	std::vector<FOWCommand> command_queue;

	GridCharacterState state;
	GridManager *grid_manager;
	bool selected;

};