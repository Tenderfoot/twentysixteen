
#include "spine_entity.h"


void SpineEntity::init()
{
	spine_data.load_spine_data("everybody");
}


void SpineEntity::draw()
{
	glPushMatrix();
		glTranslatef(position.x, position.y-1.5, position.z);
		if(flip)
			glRotatef(180, 0, 1, 0);
		glScalef(0.006f, 0.006f, 0.006f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
			spine_data.draw();
		Paintbrush::stop_shader();
	glPopMatrix();
}