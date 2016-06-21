#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../particles.h"
#include "../spine_data.h"
#include "../VFXGrass.h"
#include "../modelprop_entity.h"
#include "../player_entity.h"

typedef enum
{
	TYPE_ENTITY,
	TYPE_FACE
}rendertarget_types;

class render_target
{
public:
	rendertarget_types type;
	t_vertex position;
	Entity *the_entity;
	t_face face;
	GLuint texture;
};

class SceneTechDemo : public BaseTechDemo
{
public:
	SceneTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	float rotation;
	void reset();

	// Level Entities
	std::vector<Entity*> entities;
	std::vector<render_target> render_targets;

	static t_vertex camera_position;

	struct by_depth_rendertarget {
		bool operator()(render_target left, render_target right) {
			return left.position.z < right.position.z;
		}
	};

	ParticleEmitter myemitter;
	ModelData level_static;
	PlayerEntity spineboy;
};