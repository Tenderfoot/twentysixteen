
#include "scenetechdemo.h"

t_vertex SceneTechDemo::camera_position = t_vertex(0, 0, 0);

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

	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_static.model, t_vertex(0, 0, 0), 0);

	int i, j, k;
	printf("=========\n"); 
	printf("grass entities generated: %d\n", grass_entities.size());
	printf("=========\n");

	for (i = 0; i < grass_entities.size(); i++)
	{
		entities.push_back(grass_entities.at(i));
	}

	render_target current_target;
	float x, y, z;
	for (i = 0; i < level_static.model->meshes.size(); i++)
	{
		for (j = 0; j < level_static.model->meshes.at(i)->faces.size(); j++)
		{
			current_target.type = TYPE_FACE;
			current_target.face = *level_static.model->meshes.at(i)->faces.at(j);
			current_target.texture = level_static.model->textures[level_static.model->meshes.at(i)->faces.at(j)->material_index];

			// calculate position of face
			x = y = z = 0;
			z = 9999999999;
			for (k = 0; k < current_target.face.verticies.size(); k++)
			{
				x += current_target.face.verticies.at(k).x;
				y += current_target.face.verticies.at(k).y;

				if(z>current_target.face.verticies.at(k).z)
					z = current_target.face.verticies.at(k).z;
			}
			x = x / current_target.face.verticies.size();
			y = y / current_target.face.verticies.size();
			//z = z / current_target.face.verticies.size();
			
			current_target.position = t_vertex(x, y, z);

			render_targets.push_back(current_target);
		}
	}

	for (i = 0; i < entities.size(); i++)
	{
		current_target.type = TYPE_ENTITY;
		current_target.the_entity = entities.at(i);
		current_target.position = entities.at(i)->position;
		render_targets.push_back(current_target);
	}

	printf("rendertargets size: %d\n", render_targets.size());

	std::sort(render_targets.begin(), render_targets.end(), SceneTechDemo::by_depth_rendertarget());

	/* I still might need this later
	// Test Modelprop
	ModelPropEntity *NewEntity = new ModelPropEntity;
	NewEntity->init();
	entities.push_back(NewEntity);
	*/
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
			x = x - (time_delta / 200);
			flip = false;
		}

		if (keydown_map[RIGHT])
		{
			x = x + (time_delta / 200);
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

	camera_position.x = x-x;
	camera_position.y = 12;
	camera_position.z = 10-25;

	gluLookAt(x, 12, 10, x, 10, -25, 0, 1, 0);

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 3;
	LightManager::lights[0].z = -13;
	LightManager::lights[0].radius = 12;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;

	// Star Field

	glPushMatrix();
		glTranslatef(x/1.25, 0.0f, -50.0f);
		for (i = 0; i < myemitter.particles.size(); i++)
		{
			myemitter.particles.at(i)->draw();
		}
	glPopMatrix();


	// The Level
	/*
	glPushMatrix();
		glTranslatef(0.0f, -10.0f, -10.0f);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();
	*/

	glPushMatrix();
		for (i = 0; i < render_targets.size(); i++)
		{
			if (render_targets.at(i).type == TYPE_FACE)
			{
				Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
				glPushMatrix();
					Paintbrush::draw_face(render_targets.at(i).face, render_targets.at(i).texture);
				glPopMatrix();
				Paintbrush::stop_shader();
			}
			else
			{
				glPushMatrix();
					render_targets.at(i).the_entity->draw();
				glPopMatrix();
			}
		}
	glPopMatrix();


	// Entities pre-game plane
/*
	glPushMatrix();
		glTranslatef(0.0f, -10.0f, 0.0f);
		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->position.z < -10)
				entities.at(i)->draw();
		}
	glPopMatrix();
*/
	// Gameplane drawing

	glPushMatrix();
		glTranslatef(x, y+6.5, -2.0f);
		glScalef(0.006f, 0.006f, 0.006f);
		glRotatef(180 * flip, 0, 1, 0);
		Paintbrush::use_shader(Paintbrush::get_shader("point_light_spine"));
		spineboy.draw();
		Paintbrush::stop_shader();
	glPopMatrix();

	// Post-gameplane entities
/*
	glPushMatrix();
		glTranslatef(0.0f,-10.0f, 0.0f);
		for (i = 0; i < entities.size(); i++)
		{
			if(entities.at(i)->position.z > -10)
				entities.at(i)->draw();
		}
	glPopMatrix();
*/

	BaseTechDemo::draw();
}
