
#include "scenetechdemo.h"

void SceneTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Scene Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("scenetest.fbx", 0.01);

	spineboy.load_spine_data("spineboy");
}

void SceneTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);
	spineboy.update_skeleton(time_delta);
}

void SceneTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void SceneTechDemo::draw()
{
	gluLookAt(sin(rotation / 200) * 0.5, cos(rotation / 500) * 1, 0, 0, 0, -25, 0, 1, 0);

	glPushMatrix();
		glTranslatef(0.0f, -10.0f, -10.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-5.0f, 1.5f, -7.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		Paintbrush::draw_some_grass();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(5.0f, 1.5f, -7.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		Paintbrush::draw_some_grass();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -3.3f, -10.0f);
	glScalef(0.005f, 0.005f, 0.005f);
	spineboy.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0f, 1.5f, 9.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	Paintbrush::draw_some_grass();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0f, 1.5f, 9.0f);
	glScalef(2.0f, 2.0f, 2.0f);
	Paintbrush::draw_some_grass();
	glPopMatrix();

	BaseTechDemo::draw();
}
