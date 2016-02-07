#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "entity.h"

class GameEntity : public Entity
{
public:
	void draw();
	void update(float delta_time);
};