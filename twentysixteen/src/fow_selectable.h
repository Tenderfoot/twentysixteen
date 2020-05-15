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

	t_vertex position;
	t_ability_enum type;
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

	FOWCommand current_command;

	std::vector<FOWCommand> command_queue;

	GridCharacterState state;
	GridManager *grid_manager;
	bool selected;

};