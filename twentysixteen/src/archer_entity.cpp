
#include "archer_entity.h"

void ArcherEntity::update(float delta_time)
{
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}

void ArcherEntity::init()
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, "knight");
	spine_data.animation_name = "shoot";
	spine_data.start_time = SDL_GetTicks();

	current_arrow = new ArrowEntity();
	current_arrow->position = position;

	add_entity(current_arrow);
}


void ArcherEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.001;
	}

}