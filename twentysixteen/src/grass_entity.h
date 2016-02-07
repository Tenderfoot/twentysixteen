#pragma once

/* Class GrassEntity */
// This represents a grass polygon with appropriate texcoords

#include "common.h"
#include "model_data.h"
#include "entity.h"

class GrassEntity : public Entity
{
public:
	
	// the left and right vertex
	t_vertex v1, v2;
	float xtexcoord1, xtexcoord2;

	void draw()
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
			glBegin(GL_QUADS);
				glTexCoord2f(xtexcoord2, 1.00f);	glVertex3f(v2.x, v2.y+1, v2.z);
				glTexCoord2f(xtexcoord1, 1.00f);	glVertex3f(v1.x, v1.y+1, v1.z);
				glTexCoord2f(xtexcoord1, 0.0f);		glVertex3f(v1.x, v1.y, v1.z);
				glTexCoord2f(xtexcoord2, 0.0f);		glVertex3f(v2.x, v2.y, v2.z);
			glEnd();
		glPopMatrix();
	}

	void update(float delta_time)
	{
	}
};