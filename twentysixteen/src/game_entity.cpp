
#include "game_entity.h"

void GameEntity::update(float delta_time)
{
	if (apply_friction)
	{
		if (velocity.x > 0)
		{
			velocity.x -= (0.0000125);
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if (velocity.x < 0)
		{
			velocity.x += (0.0000125);
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
	}
	apply_friction = false;

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.0001;
	}

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}

void GameEntity::correct_against_collisiongroup(t_collisiongroup collision_group, float time_delta)
{
	real_velocity = t_vertex(velocity.x*time_delta, velocity.y*time_delta, 0.0f);

	bool intersected = false;

	int i, j;

	for (i = 0; i < collision_group.collision_groups.size(); i++)
	{
		PolygonCollisionResult r = LinearAlgebra::PolygonCollision(return_polygon(), collision_group.collision_groups.at(i), real_velocity);

		if (r.WillIntersect) {
			// Move the polygon by its velocity, then move
			// the polygons appart using the Minimum Translation Vector

			position.x += r.MinimumTranslationVector.x;
			position.y += r.MinimumTranslationVector.y;

			if (r.MinimumTranslationVector.y > 0)
			{
				velocity.y = 0;
				apply_friction = true;
			}

			if (r.MinimumTranslationVector.x != 0 && r.MinimumTranslationVector.y == 0)
			{
				velocity.x = 0;
			}

			if (r.MinimumTranslationVector.y < 0)
				velocity.y = -0.001;

			intersected = true;
		}
	}
	

}