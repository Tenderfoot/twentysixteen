
#include "entity.h"

void Entity::draw()
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z+1);
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

void Entity::add_entity(Entity* entity_to_add)
{
	game_entities->push_back(entity_to_add);
}