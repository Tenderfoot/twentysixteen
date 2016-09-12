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
	
	GameEntity()
	{
		type = GAME_ENTITY;
	}

	GameEntity(t_vertex new_pos, t_vertex new_size, t_vertex new_color)
	{
		type = GAME_ENTITY;
		position = new_pos;
		size = new_size;
		color = new_color;
		initial_position = new_pos;
		velocity = t_vertex(0, 0, 0);
	}

	// An Entity has a position - a GameEntity also has:
	// -a width and height
	// -velocty
	t_vertex velocity;
	t_vertex real_velocity;
	bool apply_friction;

	void update(float time_delta);

	// activation stuff (buttons, portcullises)
	virtual void activate()
	{}
	int activate_id;
	bool activated;

	// collision stuff
	void correct_against_collisiongroup(t_collisiongroup collision_group, float time_delta);
	bool check_against_game_entity(GameEntity *opposing_entity);

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