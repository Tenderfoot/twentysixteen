
#include "enemytechdemo.h"

void EnemyTechDemo::init()
{
	init_level("meadow");

	build_render_targets();
}

void EnemyTechDemo::run(float time_delta)
{
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