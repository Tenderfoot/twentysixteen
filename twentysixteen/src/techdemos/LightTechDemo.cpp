
#include "lighttechdemo.h"

void LightTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Lights Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testchamber.fbx", 0.05);
	level_static.shader = Paintbrush::get_shader("point_light");

	spineboy.load_spine_data("spineboy");
}

void LightTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);
	rotation += (time_delta / 1000);
}

void LightTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void LightTechDemo::draw()
{
	glEnable(GL_LIGHTING);
	GLfloat light_position[] = { sin(rotation)*10, cos(rotation)*10, -20+(sin(rotation*3)*5), 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -3.5f, -22.0f);
		glScalef(0.01f, 0.01f, 0.01f);
		Paintbrush::use_shader(level_static.shader);
		spineboy.draw();
		Paintbrush::stop_shader();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(sin(rotation) * 10, cos(rotation) * 10, -20+(sin(rotation*3) * 5));
		glScalef(0.2f, 0.2f, 0.2f);
		Paintbrush::draw_cube();
	glPopMatrix();

	glDisable(GL_LIGHTING);

	BaseTechDemo::draw();
}
