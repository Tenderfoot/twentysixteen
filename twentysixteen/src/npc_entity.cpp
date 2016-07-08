
#include "npc_entity.h"

void NPCEntity::update(float time_delta)
{
	if(dead)
	{
		velocity.x = 0;
	}
	else
	{
		if (turn)
		{
			flip = false;
			velocity.x = -(0.0025);
		}
		else
		{
			flip = true;
			velocity.x = (0.0025);
		}
	}

	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}

void NPCEntity::die()
{
	if (!dead)
	{
		spine_data.animation_name = "die";
		spine_data.start_time = SDL_GetTicks();
		spine_data.looping = false;
		dead = true;
	}
}

void NPCEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	if (dead)
	{
	}
	else
	{
		spine_data.animation_name = "walk_two";

		if (velocity.y > -0.003)
		{
			velocity.y -= 0.00001;
		}
	}

}