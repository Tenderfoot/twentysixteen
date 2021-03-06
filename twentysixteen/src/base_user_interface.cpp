
#include "base_user_interface.h"

void TextWidget::draw()
{
	Paintbrush::draw_text(text, x*res_width, y*res_height, width*res_width, height*res_height);
}

void UIImage::draw()
{
	glPushMatrix();

		glTranslatef(x*res_width, y*res_height, 0.0f);
		glScalef(width*res_width, height*res_height, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture);
		if(texture == NULL)
			glColor3f(0.0, 0.0f, 0.0f);
		Paintbrush::draw_quad();
		glColor3f(1.0, 1.0f, 1.0f);

	glPopMatrix();
}

void CharacterWidget::draw()
{
	x = (res_width / 20) * 15;
	y = res_height;
	glPushMatrix();
	if (character != NULL)
		if (character->type == FOW_BUILDING || character->type == FOW_TOWNHALL || character->type == FOW_GOLDMINE)
		{
			// building spine template is positioned differently than character
			glTranslatef(x-40.0f, y-150.0f, 0.0f);
			glRotatef(180, 0.0f, 1.0f, 0.0f);
			glRotatef(180, 0.0f, 0.0f, 1.0f);
			glScalef(0.5f, 0.5f, 0.5f);
			character->spine_data.draw();
		}
		else
		{
			glTranslatef(x, y, 0.0f);
			glScalef(0.25, -0.25, 1.0f);
			character->spine_data.draw();
		}
	glPopMatrix();
}

void AbilityButton::draw()
{
	glPushMatrix();

	glTranslatef(x*res_width, y*res_height, 0.0f);
	glScalef(width*res_width, height*res_height, 1.0f);

	glBindTexture(GL_TEXTURE_2D, tex);
	if (tex == NULL)
		if (active == true)
			glColor3f(0, 1, 0);
		else
			glColor3f(color.x, color.y, color.z);
	Paintbrush::draw_quad();
	glColor3f(1.0, 1.0f, 1.0f);

	glPopMatrix();
}

void AbilityBar::draw()
{
	ability_buttons[0]->draw();
	ability_buttons[1]->draw();
}

void CombatLog::draw()
{

	x = (res_width / 8)*7.175;
	y = (res_height / 16) * 15.7;
	width = res_width / 4;
	height = (res_height / 4);

	glPushMatrix();

/*	glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(width, height, 1.0f);

		glBindTexture(GL_TEXTURE_2D, NULL);
		glColor4f(0.0, 0.0f, 0.0f, 0.5f);
		Paintbrush::draw_quad();
		glColor3f(1.0, 1.0f, 1.0f);
	glPopMatrix();
*/

	//glTranslatef(x, y, 0.0f);
	//glScalef(width, height, 1.0f);

	glColor3f(1.0, 1.0f, 1.0f);

	while (log->size() > 6)
		log->erase(log->begin());

	int i;
	for (i = 0; i < log->size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(std::string(log->at(i)), true, false));
		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
		Paintbrush::draw_text(log->at(i), x, y + ((height / 12) * (-5 + i)), ((w / h)*(height / 12)), height / 12);
	}

	
	glPopMatrix();
}

void ListWidget::draw()
{
	int i;
	for (i = 0; i < list_items.size(); i++)
	{
		if (i == current_selection)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(std::string(list_items.at(i)), true, false));
		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

		Paintbrush::draw_text(list_items.at(i), x*res_width, (y + (0.1*i))*res_height, width*res_width, height*res_height);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void MapWidget::draw()
{
	// this is for safety
	if (map_grid != NULL)
	{
		width = (res_width / 14) / (map_grid->width/15);
		height = ((res_height / 12) / (map_grid->height/15));
	}

	int i, j;
	for (i = 0; i < map_grid->width; i++)
	{
		for (j = 0; j < map_grid->height; j++)
		{
			glPushMatrix();

			glTranslatef(x+(i*0.1*width),y+(j*0.1*height), 0.0f);
			glScalef(0.1*width, 0.1*height, 1.0f);
			
			glBindTexture(GL_TEXTURE_2D, NULL);
			if (map_grid->tile_map[i][j].discovered == false)
				glColor3f(0.0, 0.0f, 0.0f);
			else if(map_grid->tile_map[i][j].wall == 1)
				glColor3f(1.0, 0.0f, 0.0f);
			else if(map_grid->entity_on_position(t_vertex(i,0,j)) != -1)
				glColor3f(0.0, 0.0f, 1.0f);
			else if (map_grid->tile_map[i][j].in_path)
				glColor3f(1.0, 1.0f, 0.0f);
			else if(map_grid->tile_map[i][j].visible == false)
				glColor3f(0.5, 0.5f, 0.5f);
			else
				glColor3f(1.0, 1.0f, 1.0f);
			Paintbrush::draw_quad();
			glColor3f(1.0, 1.0f, 1.0f);

			glPopMatrix();
		}
	}
}

void GreenBox::draw()
{
	if (visible)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.5f, 1.0f, 0.5f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(width, y);
			glVertex2f(x, y);
			glVertex2f(x, height);
			glVertex2f(width, y);
			glVertex2f(width, height);
			glVertex2f(x, height);
			glVertex2f(width, height);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void BaseUserInterface::draw()
{
	// set up orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, res_width, res_height, 0.0, -1.0, 1.0);

	// go back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glColor3f(1.0f, 1.0f, 1.0f);

	int i;
	for (i = 0; i < widgets.size(); i++)
	{
		if (widgets.at(i)->visible)
		{
			widgets.at(i)->draw();
		}
	}

	// go back to regular projection...
	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	gluPerspective(80, (float)res_width / (float)res_height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);  // Select The Model View Matrix
	glLoadIdentity();    // Reset The Model View Matrix
}

// returns whetehr or not
int BaseUserInterface::mouse_focus()
{
	int i;
	float width;
	float height;
	
	for (i = 0; i < widgets.size(); i++)
	{
		if (widgets.at(i)->coords_in_ui(mouse_coords))
		{
			return i;
		}
	}
	
	return -1;
}

void BaseUserInterface::add_widget(UIWidget *new_widget)
{
	widgets.push_back(new_widget);
}