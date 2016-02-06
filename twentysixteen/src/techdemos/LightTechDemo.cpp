
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
	gluLookAt(sin(rotation) * 3, cos(rotation*2) * 3, 0, 0, 0, -25, 0, 1, 0);

	//GLfloat light_position[] = { sin(rotation)*10, cos(rotation)*10, -20+(sin(rotation*3)*5), 0.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	while (LightManager::lights.size() < 3)
	{
		// make sure there are actually two lights to manipulate
		LightManager::lights.push_back(Light(0, 0, 0, 20));
	}

	// light 1
	LightManager::lights[0].x = sin(rotation) * 10;
	LightManager::lights[0].y = cos(rotation) * 10;
	LightManager::lights[0].z = -20 + (sin(rotation * 3) * 5);
	LightManager::lights[0].radius = 20;
	// make this motherfucker red
	LightManager::lights[0].r = 1;
	LightManager::lights[0].g = 0;
	LightManager::lights[0].b = 0;


	// light 2
	LightManager::lights[1].x = -sin(rotation/2) * 10;
	LightManager::lights[1].y = cos(rotation/2) * 10;
	LightManager::lights[1].z = -20 + (sin((rotation/2) * 3) * 5);
	LightManager::lights[1].radius = 20;
	// this one green
	LightManager::lights[1].g = 1;
	LightManager::lights[1].r = 0;
	LightManager::lights[1].b = 0;

	// light 3
	LightManager::lights[2].x = 0;
	LightManager::lights[2].y = cos(rotation) * 10;
	LightManager::lights[2].z = -22+sin(rotation) * 10;
	LightManager::lights[2].radius = 20;
	// this one blue
	LightManager::lights[2].b = 1;
	LightManager::lights[2].r = 0;
	LightManager::lights[2].g = 0;

	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -3.8f, -22.0f);
		glScalef(0.01f, 0.01f, 0.01f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
		spineboy.draw();
		Paintbrush::stop_shader();
	glPopMatrix();

	int i;
	glEnable(GL_DEPTH_TEST);
	for (i = 0; i < LightManager::lights.size(); i++)
	{
		glPushMatrix();
			glTranslatef(LightManager::lights[i].x, LightManager::lights[i].y, LightManager::lights[i].z);
			glScalef(0.2f, 0.2f, 0.2f);
			Paintbrush::draw_cube();
		glPopMatrix();
	}

	BaseTechDemo::draw();
}
