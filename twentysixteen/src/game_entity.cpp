
#include "game_entity.h"

void GameEntity::update(float time_delta)
{
}

bool GameEntity::check_against_game_entity(GameEntity *opposing_entity)
{
	PolygonCollisionResult r = LinearAlgebra::PolygonCollision(return_polygon(), opposing_entity->return_polygon(), real_velocity);

	if (r.Intersect)
	{
		return true;
	}

	return false;
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