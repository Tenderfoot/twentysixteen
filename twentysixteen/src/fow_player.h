#pragma once

#include "linear_algebra.h"
#include "./techdemos/fow_character.h"

class FOWPlayer
{
public:

	FOWPlayer()
	{
	}

	t_vertex gridstart_ui;
	t_vertex gridstart_world;

	FOWCharacter *selection;

};
#pragma once
