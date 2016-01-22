
#include "ttftechdemo.h"

void TTFTechDemo::run()
{
}

void TTFTechDemo::draw()
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

	// draw the text
	glPushMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(512.0f, 384.0f, 0.0f);
		glScalef(700, 300, 1.0f);

		glBindTexture(GL_TEXTURE_2D, Paintbrush::font_texture);
		Paintbrush::draw_quad();
	
	glPopMatrix();
}
