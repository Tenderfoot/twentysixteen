
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

		Paintbrush::draw_text(list_items.at(i), x*res_width, (y + (0.1*i))*res_height, width*res_width, height*res_height);

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

void BaseUserInterface::add_widget(UIWidget *new_widget)
{
	widgets.push_back(new_widget);
}