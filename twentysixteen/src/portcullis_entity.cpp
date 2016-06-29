#include "portcullis_entity.h"

void PortcullisEntity::draw()
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glScalef(10, size.y, 1);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(color.x, color.y, color.z);
		Paintbrush::draw_quad();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void PortcullisEntity::update(float time_delta)
{
	if (activated)
	{
		if (size.y > 0)
		{
			size.y -= 0.005*time_delta;
			position.y -= 0.005*time_delta;
		}
	}
}

void PortcullisEntity::activate()
{
	activated = true;
}
