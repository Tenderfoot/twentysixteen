
#include "entity.h"

void Entity::draw()
{
	glEnable(GL_BLEND);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glScalef(size.x, size.y, size.z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(color.x, color.y, color.z);
		Paintbrush::draw_quad();
	glPopMatrix();
	glDisable(GL_BLEND);
}

void Entity::update(float delta_time)
{
}

void Entity::reset()
{
	position = initial_position;
}