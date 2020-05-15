#pragma once

#include "linear_algebra.h"
#include "./techdemos/fow_character.h"

typedef struct
{
	std::vector<FOWCharacter*> selected_characters;
} t_selectiongroup;

class FOWPlayer
{
public:

	FOWPlayer()
	{
	}

	t_vertex gridstart_ui;
	t_vertex gridstart_world;
	t_selectiongroup selection_group;

	FOWCharacter *selection;

};