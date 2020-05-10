#pragma once

#include "../gridcharacter.h"


class FOWCharacter : public GridCharacter
{
public:

	FOWCharacter()
	{
		type = GRID_CHARACTER;
		abilities.push_back(0);
		abilities.push_back(1);
		active_ability = 0;
		visible = true;
	}


	void draw()
	{
		if (state == GRID_IDLE)
		{
			draw_position = position;
		}

		if (visible)
		{
			glPushMatrix();
			glTranslatef(draw_position.x * 5, draw_position.y+0.5, draw_position.z * 5);
			glRotatef(-90, 1.0f, 0.0f, 0.0f);
			glScalef(0.01f, 0.01f, 0.01f);
			spine_data.draw();
			glPopMatrix();
		}
	}

};
