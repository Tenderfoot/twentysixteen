
#include "player_entity.h"
#include "skeleton_entity.h"

void PlayerEntity::handle_keypress(boundinput input, bool type)
{
	keydown_map[input] = type;

	if (input == EDITOR_T && type == true)
	{
		SkeletonEntity *test = new SkeletonEntity();
		test->position = position;
		test->size = t_vertex(1, 3, 1);
		test->game_entities = game_entities;
		test->init("skel");
		add_entity(test);
	}
}

void PlayerEntity::update(float delta_time)
{
	if (apply_friction && keydown_map[LEFT] == false && keydown_map[RIGHT] == false)
	{
		if (velocity.x > 0)
		{
			velocity.x -= (0.0005);
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if (velocity.x < 0)
		{
			velocity.x += (0.0005);
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
	}
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}


void PlayerEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	if (keydown_map[LEFT] || keydown_map[RIGHT])
	{
		spine_data.animation_name = "walk_two";

		if (keydown_map[LEFT])
		{
			velocity.x -= (0.003);
			spine_data.flip = false;
		}

		if (velocity.x < -0.0125)
			velocity.x = -0.0125;

		if (keydown_map[RIGHT])
		{
			velocity.x += (0.003);
			spine_data.flip = true;
		}

		if (velocity.x > 0.0125)
			velocity.x = 0.0125;
	}
	else
		spine_data.animation_name = "idle";

	// jump
	if (keydown_map[UP] == true)
	{
		if (velocity.y == 0)
			velocity.y = +0.04;
	}

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.0025;
	}

}