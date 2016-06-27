
#include "enemytechdemo.h"

void EnemyTechDemo::init()
{
	skeleton.init("skel");
	skeleton.position = t_vertex(10, 30, 0);
	skeleton.size = t_vertex(1, 3, 1);
	skeleton.velocity = t_vertex(0, 0, 0);

	render_target new_entity;
	new_entity.type = TYPE_ENTITY;
	// knight
	new_entity.the_entity = &skeleton;
	new_entity.position = skeleton.position;
	render_targets.push_back(new_entity);

	build_render_targets();
}

void EnemyTechDemo::run(float time_delta)
{
	skeleton.correct_against_collisiongroup(collision_group, time_delta);
	skeleton.update(time_delta);
	skeleton.player_update(time_delta);

	BaseGameLevel::run(time_delta);
}

void EnemyTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		LightManager::lights.push_back(Light(0, 0, 0, 100));
	}

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 10;
	LightManager::lights[0].z = -15;
	LightManager::lights[0].radius = 25;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;
}