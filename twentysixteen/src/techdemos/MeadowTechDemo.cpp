
#include "meadowtechdemo.h"

void MeadowTechDemo::init()
{
	init_level("meadow");
	build_render_targets();
}

void MeadowTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		LightManager::lights.push_back(Light(0, 0, 0, 100));
	}

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 10;
	LightManager::lights[0].z = -20;
	LightManager::lights[0].radius = 35;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;
}
