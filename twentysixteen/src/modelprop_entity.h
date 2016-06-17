#pragma once

/* Class GrassEntity */
// This represents a grass polygon with appropriate texcoords

#include "common.h"
#include "model_data.h"
#include "entity.h"
#include "paintbrush.h"

class ModelPropEntity : public Entity
{
public:
	t_3dModel *mymodel;

	void draw()
	{
		glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
			Paintbrush::draw_model(mymodel);
			Paintbrush::stop_shader();
		glPopMatrix();
	}

	void init()
	{
		mymodel = ModelData::import("pepsi.fbx", 0.004);
		position.z = -18;
		position.y = 6;
	}

	void update(float delta_time)
	{
	}

}; 