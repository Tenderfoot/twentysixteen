
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

	if (keydown_map[LEFT] || keydown_map[RIGHT])
	{
		spineboy.animation_name = "walk";

		if (keydown_map[LEFT])
		{
			x = x - (time_delta / 300);
			flip = true;
		}

		if (keydown_map[RIGHT])
		{
			x = x + (time_delta / 300);
			flip = false;
		}
	}
	else
		spineboy.animation_name = "idle";
}

void SceneTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		// make sure there are actually two lights to manipulate
		LightManager::lights.push_back(Light(0, 0, 0, 20));
	}
}

void SceneTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input]=type;

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void SceneTechDemo::draw()
{
	gluLookAt(x+sin(rotation / 200) * 0.5, cos(rotation / 500) * 1, 0, x, 0, -25, 0, 1, 0);

	LightManager::lights[0].x = -x + sin(rotation / 100) * 4;
	LightManager::lights[0].y = cos(rotation / 100) * 1;
	LightManager::lights[0].z = -10 + (sin(rotation / 100 * 3) * 5);
	LightManager::lights[0].radius = 20;

	LightManager::lights[0].r = 1;
	LightManager::lights[0].g = 0;
	LightManager::lights[0].b = 0;

	glPushMatrix();
		glTranslatef(0.0f, -10.0f, -10.0f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
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
	glTranslatef(x, y-3.3f, -10.0f);
	glScalef(0.005f, 0.005f, 0.005f);
	glRotatef(180 * flip, 0, 1, 0);
	Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
	spineboy.draw();
	Paintbrush::stop_shader();
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
