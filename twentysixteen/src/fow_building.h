#pragma once

#include "fow_selectable.h"

class FOWBuilding : public FOWSelectable
{
public:

	FOWBuilding()
	{
		size = 1;
	}

	FOWBuilding(int x, int z, int size)
	{
		spine_data.load_spine_data("buildings");
		position.x = x;
		position.z = z;
		this->size = size;
	}

	void draw()
	{
		glPushMatrix();
			glTranslatef(position.x * 5, position.y + 0.01, (position.z * 5)+0.15);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glScalef((0.023f)*size, (0.023f)*size, (0.023f)*size);
			spine_data.draw();
		glPopMatrix();
	}

	int size;

};