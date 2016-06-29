
#include "entity.h"

void Entity::draw()
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glScalef(size.x, size.y, size.z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(color.x, color.y, color.z);
		Paintbrush::draw_quad();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Entity::update(float time_delta)
{
}

void Entity::reset()
{
	position = initial_position;
}

void Entity::add_entity(Entity* entity_to_add)
{
	game_entities->push_back(entity_to_add);
}