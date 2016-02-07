#pragma once

/* Class Entity */
// So this is kind of another big design decision.
// I want to have "SceneEntity" and "GameEntity" that both
// inherit from class Entity.
//
// The idea is SceneEntity will be foreground / background props
// and GameEntity will be anything that exists in the game plane.
// it includes a transform, as well as a virtual draw and update function.
// Entity itself is abstract.
//
// The main game loop will z-sort and draw all of a levels entities.
//
// The GameEntity class will contain collision, and the Player class will
// extend from GameEntity.
//
// The SceneEntity class will include things like Grass or placing lights
// that have been allocated by the light_manager.

#include "common.h"
#include "model_data.h"


class Entity
{
public:
	t_vertex position;
	virtual void draw()=0;
	virtual void update(float delta_time)=0;
};

struct by_depth {
	bool operator()(Entity *left, Entity *right) {
		return left->position.z < right->position.z;
	}
};