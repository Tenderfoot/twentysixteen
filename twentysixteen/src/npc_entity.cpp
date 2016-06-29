
#include "npc_entity.h"

void NPCEntity::update(float delta_time)
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

	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}


void NPCEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	spine_data.animation_name = "walk_two";

	if (velocity.y > -0.003)
	{
		velocity.y -= 0.00001;
	}

}