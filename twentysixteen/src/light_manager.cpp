
#include "light_manager.h"

// shaders and uniform storage
std::vector<Light> LightManager::lights = {};

void LightManager::init()
{
	Light new_light(0,0,0,20);
	new_light.radius = 20;
	lights.push_back(new_light);
}

void LightManager::reset()
{
	lights.clear();
}