#pragma once

/* Class LightManager */
// The idea for the light manager is
// for it to be a static class that controls
// the available lights. It will let you set
// the ambient light, place colored lights
// of different kinds (point lights, spot lights),
// reset the lighting for a level, and apply "default"
// lighting of 100% ambient for test levels.

#include <SDL_opengl.h>

#include <spine/spine.h>
#include <spine/extension.h>

#include <vector>

#include "common.h"

class Light
{
public:
	Light(float x, float y, float z, float radius) : x(x), y(y), z(z), radius(radius)
	{
		// default white
		r = 1;
		g = 1;
		b = 1;
	};

	float x, y, z;
	float r, g, b;
	float radius;
	light_types type;
};

class LightManager
{
public:
	static void init();
	static std::vector<Light> lights;
};

