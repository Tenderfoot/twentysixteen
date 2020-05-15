#pragma once

#include "fow_selectable.h"

class FOWBuilding : public FOWSelectable
{
public:

	FOWBuilding()
	{
	}

	FOWBuilding(int x, int z, int size)
	{
		type = FOW_BUILDING;
		spine_data.load_spine_data("buildings");
		position.x = x;
		position.z = z;
		this->size = size;
	}

	void draw()
	{
		glPushMatrix();
			glTranslatef((position.x * 5)-2.5, position.y + 0.01, (position.z * 5)-2.5);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glScalef((0.02275f)*size, (0.02275f)*size, (0.02275f)*size);
			spine_data.draw();
		glPopMatrix();
	}

	int size;

};