
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
	
	Paintbrush::draw_some_grass();

	BaseTechDemo::draw();
}
