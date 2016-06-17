
#include "scenetechdemo.h"

void SceneTechDemo::init()
{
	while (myemitter.particles.size() < 1000)
	{
		myemitter.particles.push_back(new Star);
	}

	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(-100, 0, 0), t_vertex(200, 75, 0));

	level_static.model = ModelData::import("scenetest.fbx", 0.01);

	spineboy.load_spine_data("everybody");

	// so basically what you're gunna want to do.... is iterate through the faces in the level, and create a series of SceneEntities based on
	// the grass textured faces
	// then, iterating through and drawing these in draw - you can z-sort
	
	// now that you have it drawing a quad based on an entity -
	// now, plane slice the level and make grass entities for each edge
	// from grass textured faces...

	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_static.model, t_vertex(0, 0, -10), 0);

	int i;
	printf("=========\n"); 
	printf("grass entities generated: %d\n", grass_entities.size());
	printf("=========\n");

	for (i = 0; i < grass_entities.size(); i++)
	{
		entities.push_back(grass_entities.at(i));
	}

	// Test Modelprop
	ModelPropEntity *NewEntity = new ModelPropEntity;
	NewEntity->init();
	entities.push_back(NewEntity);
}

void SceneTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);
	spineboy.update_skeleton(time_delta);

	if (keydown_map[LEFT] || keydown_map[RIGHT])
	{
		spineboy.animation_name = "walk_two";

		if (keydown_map[LEFT])
		{
			x = x - (time_delta / 300);
			flip = false;
		}

		if (keydown_map[RIGHT])
		{
			x = x + (time_delta / 300);
			flip = true;
		}
	}
	else
		spineboy.animation_name = "idle";

	myemitter.update(time_delta);
}

void SceneTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		// make sure there are actually two lights to manipulate
		LightManager::lights.push_back(Light(0, 0, 0, 100));
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
	int i;

	gluLookAt(x, 2, 0, x, 0, -25, 0, 1, 0);

	std::sort(entities.begin(), entities.end(), by_depth());

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 5;
	LightManager::lights[0].z = -2;
	LightManager::lights[0].radius = 25;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;

	// Star Field

	glPushMatrix();
		glTranslatef(x/1.5, 0.0f, -50.0f);
		for (i = 0; i < myemitter.particles.size(); i++)
		{
			myemitter.particles.at(i)->draw();
		}
	glPopMatrix();


	// The Level

	glPushMatrix();
		glTranslatef(0.0f, -10.0f, -10.0f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	// Entities pre-game plane
	glPushMatrix();
		glTranslatef(0.0f, -10.0f, 0.0f);
		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->position.z < -10)
				entities.at(i)->draw();
		}
	glPopMatrix();

	// Gameplane drawing

	glPushMatrix();
		glTranslatef(x, y-3.5, -12.0f);
		glScalef(0.006f, 0.006f, 0.006f);
		glRotatef(180 * flip, 0, 1, 0);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
		spineboy.draw();
		Paintbrush::stop_shader();
	glPopMatrix();

	// Post-gameplane entities

	glPushMatrix();
		glTranslatef(0.0f,-10.0f, 0.0f);
		for (i = 0; i < entities.size(); i++)
		{
			if(entities.at(i)->position.z > -10)
				entities.at(i)->draw();
		}
	glPopMatrix();


	BaseTechDemo::draw();
}
