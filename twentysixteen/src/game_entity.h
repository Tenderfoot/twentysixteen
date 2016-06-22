#pragma once

/* Class GameEntity */
// This will be stuff in the world plane
// handles simple physics, collision

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "entity.h"

class GameEntity : public Entity
{
public:

	// An Entity has a position - a GameEntity also has:
	// -a width and height
	// -velocty
	t_vertex velocity;
	t_vertex real_velocity;

	bool apply_friction;

	void draw();
	void update(float time_delta);

	void correct_against_collisiongroup(t_collisiongroup collision_group, float time_delta);

	t_polygon return_polygon()
	{
		t_polygon to_return;
		t_edge edge;

		edge.verticies.push_back(t_vertex(position.x + (size.x / 2), position.y + (size.y / 2),0));
		edge.verticies.push_back(t_vertex(position.x + (size.x / 2), position.y - (size.y / 2), 0));
		to_return.edges.push_back(edge);
		edge.verticies.clear();

		edge.verticies.push_back(t_vertex(position.x + (size.x / 2), position.y - (size.y / 2), 0));
		edge.verticies.push_back(t_vertex(position.x - (size.x / 2), position.y - (size.y / 2), 0));
		to_return.edges.push_back(edge);
		edge.verticies.clear();

		edge.verticies.push_back(t_vertex(position.x - (size.x / 2), position.y - (size.y / 2), 0));
		edge.verticies.push_back(t_vertex(position.x - (size.x / 2), position.y + (size.y / 2), 0));
		to_return.edges.push_back(edge);
		edge.verticies.clear();

		edge.verticies.push_back(t_vertex(position.x - (size.x / 2), position.y + (size.y / 2), 0));
		edge.verticies.push_back(t_vertex(position.x + (size.x / 2), position.y + (size.y / 2), 0));
		to_return.edges.push_back(edge);
		edge.verticies.clear();

		return to_return;
	};

};