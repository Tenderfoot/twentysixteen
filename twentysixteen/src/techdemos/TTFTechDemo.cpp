
#include "ttftechdemo.h"

void TTFTechDemo::init()
{
	BaseTechDemo::init();
	texture = Paintbrush::Soil_Load_Texture("data/images/controller_splash.png");
}

void TTFTechDemo::run()
{
}

void TTFTechDemo::draw()
{
	BaseTechDemo::draw();
	
	// test SOIL
	// set up orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1024, 768, 0.0, -1.0, 1.0);

	// go back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(512.0f, 500.0f, 0.0f);
		glScalef(700, 300, 1.0f);

		glBindTexture(GL_TEXTURE_2D, texture);
		Paintbrush::draw_quad();

	glPopMatrix();
}
