#pragma once

#include "fow_selectable.h"

enum building_types
{
	BUILDING,
	TOWNHALL,
	GOLDMINE
};

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

class FOWTownHall: public FOWBuilding
{
public:

	FOWTownHall()
	{
	}

	FOWTownHall(int x, int z, int size)
	{
		type = FOW_TOWNHALL;
		spine_data.load_spine_data("buildings");
		position.x = x;
		position.z = z;
		this->size = size;
	}

	void process_command(FOWCommand next_command)
	{
		
		if (next_command.type == BUILD_UNIT)
		{
			printf("Build Unit command recieved\n");
		}

		FOWSelectable::process_command(next_command);
	};

};

class FOWGoldMine : public FOWBuilding
{
public:

	FOWGoldMine()
	{
	}

	FOWGoldMine(int x, int z, int size)
	{
		type = FOW_GOLDMINE;
		spine_data.load_spine_data("buildings");
		position.x = x;
		position.z = z;
		this->size = size;
	}
};