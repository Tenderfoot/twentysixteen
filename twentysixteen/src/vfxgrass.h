#pragma once

/* Class VFXGrass */
// The idea for the vfxgrass is to
// plane intersect a model at 0.5 increments on the z axis
// then use that edge information to create grass polygons.

#include <SDL_opengl.h>

#include <spine/spine.h>
#include <spine/extension.h>

#include <vector>

#include "common.h"
#include "model_data.h"
#include "grass_entity.h"
#include "linear_algebra.h"

class VFXGrass
{
public:
	static std::vector<Entity*> generate_grass(t_3dModel from_model, t_vertex model_transform, int mesh_id);
};

