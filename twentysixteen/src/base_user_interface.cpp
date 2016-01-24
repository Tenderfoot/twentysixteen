
#include "base_user_interface.h"

void TextWidget::draw()
{
	Paintbrush::draw_text(text, 512, 200, 300, 300);
}

void UIImage::draw()
{
	glPushMatrix();

		glTranslatef(512.0f, 500.0f, 0.0f);
		glScalef(700, 300, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture);
		Paintbrush::draw_quad();

	glPopMatrix();
}

void BaseUserInterface::draw()
{
	// set up orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1024, 768, 0.0, -1.0, 1.0);

	// go back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glColor3f(1.0f, 1.0f, 1.0f);

	int i;
	for (i = 0; i < widgets.size(); i++)
		widgets.at(i)->draw();

	// go back to regular projection...
	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	gluPerspective(80, (float)1024 / (float)768, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);  // Select The Model View Matrix
	glLoadIdentity();    // Reset The Model View Matrix
}

void BaseUserInterface::add_widget(UIWidget *new_widget)
{
	widgets.push_back(new_widget);
}