
#include "skeleton_entity.h"

void SkeletonEntity::update(float time_delta)
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

	if (apply_friction)
		velocity.y = 0;

	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}

void SkeletonEntity::init(char *who)
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, who);
	
	dirt.position = position;
	dirt.position.y -= 2;
	dirt.size = t_vertex(5.0f, 5.0f, 1.0f);

	int i;
	for (i = 0; i < 100; i++)
		dirt.particles.push_back(new DirtParticle);

	dirt.init(Paintbrush::get_texture("data/images/fire.png", false, false), dirt.position, t_vertex(5.0f, 5.0f, 1.0f));

	game_entities->push_back(&dirt);
}

void SkeletonEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	spine_data.animation_name = "climb_out";

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.0025;
	}

}