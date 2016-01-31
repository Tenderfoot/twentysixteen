
#include "grasstechdemo.h"

void GrassTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Grass Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.5, 0.5, 0.05));

	grass = Paintbrush::get_texture("data/images/grass.png", false, true);
}

void GrassTechDemo::run(float time_delta)
{
	rotation += (time_delta / 5);
}

void GrassTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void GrassTechDemo::draw()
{
	glBindTexture(GL_TEXTURE_2D, NULL);

	glPushMatrix();
		glColor3f(0.0f, 0.5f, 0.0f);
		glTranslatef(0.0f, -2.0f, -5.0f);
		glRotatef(90, 1, 0, 0);
		glScalef(5.0f, 5.0f, 5.0f);
		Paintbrush::draw_quad();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, grass);
	// make sure it doesn't wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glPushMatrix();
		Paintbrush::use_shader(Paintbrush::get_shader("grass"));
		glPushMatrix();
			glTranslatef(0.0f, -2.0f, -8.0f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5f, -2.0f, -7.5f);
			glScalef(3.0f, 1.0f, 1.0f);

			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.5f, -2.0f, -7.0f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5f, -2.0f, -6.5f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0f, -2.0f, -6.0f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5f, -2.0f, -5.5f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.5f, -2.0f, -5.0f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5f, -2.0f, -4.5f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0f, -2.0f, -4.0f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5f, -2.0f, -3.5f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0f, -2.0f, -3.3f);
			glScalef(3.0f, 1.0f, 1.0f);
			Paintbrush::draw_quad();
		glPopMatrix();
		Paintbrush::stop_shader();
	glPopMatrix();



	BaseTechDemo::draw();
}
