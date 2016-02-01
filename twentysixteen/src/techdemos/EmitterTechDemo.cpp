
#include "emittertechdemo.h"

void EmitterTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Emitter Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testchamber.fbx", 0.05);
}

void EmitterTechDemo::run(float time_delta)
{
}

void EmitterTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void EmitterTechDemo::draw()
{
	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	BaseTechDemo::draw();
}
