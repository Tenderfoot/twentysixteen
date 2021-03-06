#pragma once

/* Class GrassEntity */
// This represents a grass polygon with appropriate texcoords

#include "common.h"
#include "model_data.h"
#include "entity.h"
#include "paintbrush.h"

class GrassEntity : public Entity
{
public:

	GrassEntity()
	{
		type = GRASS_ENTITY;
		tex_displacement = ((float)(rand() % 100)) / 100;

	}

	// the left and right vertex
	t_vertex v1, v2;
	float xtexcoord1, xtexcoord2;
	float tex_displacement;
	void draw()
	{
		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture("data/images/grass.png", false, false, true));
		glColor3f(1.0f, 1.0f, 1.0f);

		float diff = abs(v2.x - v1.x) / 10;
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glPushMatrix();
			Paintbrush::use_shader(Paintbrush::get_shader("grass"));
			glBegin(GL_QUADS);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(1*diff+tex_displacement, 1.00f);	glVertex3f(v2.x, v2.y+2, v2.z);
				glTexCoord2f(0+tex_displacement, 1.00f);	glVertex3f(v1.x, v1.y+2, v1.z);
				glTexCoord2f(0+tex_displacement, 0.0f);		glVertex3f(v1.x, v1.y, v1.z);
				glTexCoord2f(1*diff+tex_displacement, 0.0f);		glVertex3f(v2.x, v2.y, v2.z);
			glEnd();
			Paintbrush::stop_shader();
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	void update(float time_delta)
	{
	}
};