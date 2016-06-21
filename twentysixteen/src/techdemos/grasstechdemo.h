#pragma once

#include "basetechdemo.h"
#include "../model_data.h"
#include "../linear_algebra.h"
#include "../vfxgrass.h"
#include "../render_target.h"

class GrassTechDemo : public BaseTechDemo
{
public:
	GrassTechDemo()
	{
		this->techdemo_title = "Model to Polygon Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);
	void reset();
	void build_render_targets();

	float rotation;
	GLuint grass;
	ModelData level_static;	
	std::vector<Entity*> entities;
	std::vector<render_target> render_targets;

	struct by_depth_rendertarget {
		bool operator()(render_target left, render_target right) {
			return left.position.z < right.position.z;
		}
	};

};
