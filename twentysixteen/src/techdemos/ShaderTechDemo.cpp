
#include "shadertechdemo.h"

void ShaderTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Shader Test Chamber", 0.5, 0.1, 0.3, 0.1));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	test_chamber = ModelData::import("testchamber.fbx", 0.05);

	shader_program = Paintbrush::load_shader("doesn't matter");
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
	gluLookAt(sin(rotation/100)*3, cos(rotation / 250) * 3,0,0,0,-25,0,1,0);

	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::draw_model(test_chamber);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -22.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		glRotatef(rotation, 1.0f, 1.0f, 0.0f);
		Paintbrush::use_shader(shader_program);
		Paintbrush::draw_cube();
		Paintbrush::stop_shader();
	glPopMatrix();

	BaseTechDemo::draw();
}
