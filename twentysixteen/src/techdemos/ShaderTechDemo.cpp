
#include "shadertechdemo.h"

void ShaderTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Shader Test Chamber", 0.5, 0.1, 0.3, 0.1));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	test_chamber = ModelData::import("testchamber.fbx", 0.05);
}

void ShaderTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);
}

void ShaderTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void ShaderTechDemo::draw()
{
	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::draw_model(test_chamber);
	glPopMatrix();

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 25.0 };
	GLfloat light_position[] = { 0.0, 0.0, 20.0, 0.0 };

	/*glPushMatrix();
		glTranslatef(3.0f, 0.0f, -20.0f);
		glScalef(1.0f, 1.0f, 1.0f);
		Paintbrush::draw_cube();
	glPopMatrix();*/

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -25.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		glRotatef(rotation, 1.0f, 1.0f, 0.0f);
		Paintbrush::draw_cube();
	glPopMatrix();

	glDisable(GL_LIGHTING);

	BaseTechDemo::draw();
}
