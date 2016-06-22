
#include "npc_entity.h"

void NPCEntity::handle_keypress(boundinput input, bool type)
{
	//keydown_map[input] = type;
}

void NPCEntity::update(float delta_time)
{

	if (position.x > 17.5)
	{
		position.x = 17.5;
		turn = true;
	}

	if (position.x < 2.5)
	{
		position.x = 2.5;
		turn = false;
	}
	
	if (turn)
	{
		flip = false;
		velocity.x = -(delta_time*0.00025);
	}
	else
	{
		flip = true;
		velocity.x = (delta_time*0.00025);
	}

	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}


void NPCEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	spine_data.animation_name = "walk_two";

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.0001*time_delta;
	}

}