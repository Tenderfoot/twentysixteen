
#include "spine_entity.h"

void SpineEntity::init()
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, "witch");
}


void SpineEntity::init(char *who)
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, who);
}

void SpineEntity::draw()
{
	glPushMatrix();
		glTranslatef(position.x, position.y-1.5, position.z);
		glScalef(0.006f, 0.006f, 0.006f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
			spine_data.draw();
		Paintbrush::stop_shader();
	glPopMatrix();
}