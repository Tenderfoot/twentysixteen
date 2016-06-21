
#include "player_entity.h"

void PlayerEntity::handle_keypress(boundinput input, bool type)
{
	keydown_map[input] = type;
}

void PlayerEntity::player_update(float time_delta)
{
	if (keydown_map[LEFT] || keydown_map[RIGHT])
	{
		spine_data.animation_name = "walk_two";

		if (keydown_map[LEFT])
		{
			position.x = position.x - (time_delta / 200);
			flip = false;
		}

		if (keydown_map[RIGHT])
		{
			position.x = position.x + (time_delta / 200);
			flip = true;
		}
	}
	else
		spine_data.animation_name = "idle";
}