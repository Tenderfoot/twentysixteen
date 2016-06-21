
#include "emittertechdemo.h"

void EmitterTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Emitter Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testchamber.fbx", 0.05);
	level_static.shader = Paintbrush::get_shader("point_light");

	while (myemitter.particles.size() < 50)
	{
		myemitter.particles.push_back(new FireParticle);
	}
	
	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(0, 0, 0), t_vertex(3, 1, 0));
}

void EmitterTechDemo::run(float time_delta)
{
	rotation += time_delta/500;
	LightManager::lights[0].radius = 15 + abs(sin(rotation)) * 5;
	myemitter.update(time_delta);
}

void EmitterTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		// make sure there are actually two lights to manipulate
		LightManager::lights.push_back(Light(0, 0, 0, 20));
	}
}

void EmitterTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void EmitterTechDemo::draw()
{
	
	LightManager::lights[0].r = 1;
	LightManager::lights[0].g = 0;
	LightManager::lights[0].b = 0;

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 2;
	LightManager::lights[0].z = -15;

	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();
	

	glPushMatrix();
		glTranslatef(-1.0f, -3.0f, -22.0f);
		int i;
		for (i = 0; i < myemitter.particles.size(); i++)
		{
			myemitter.particles.at(i)->draw();
		}
	glPopMatrix();

	BaseTechDemo::draw();
}
