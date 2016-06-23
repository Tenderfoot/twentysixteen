
#include "entity.h"

void Entity::draw()
{
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glScalef(size.x, size.y, size.z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(color.x, color.y, color.z);
		Paintbrush::draw_quad();
	glPopMatrix();
}

void Entity::update(float delta_time)
{
}

void Entity::reset()
{
	position = initial_position;
}