
#include "scenetechdemo.h"

t_vertex SceneTechDemo::camera_position = t_vertex(0, 0, 0);

void SceneTechDemo::init()
{
	while (myemitter.particles.size() < 1000)
	{
		myemitter.particles.push_back(new Star);
	}

	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(-100, 0, 0), t_vertex(200, 75, 0));

	level_static.model = ModelData::import("brandnewscene.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	// build grass
	int i;
	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_static.model, t_vertex(0, 0, 0), 0);
	for (i = 0; i < grass_entities.size(); i++)
	{
		entities.push_back(grass_entities.at(i));
	}

	spineboy.init();
	spineboy.position = t_vertex(0, 10, 0);
	spineboy.size = t_vertex(1, 3, 1);
	spineboy.velocity = t_vertex(0, 0, 0);

	build_render_targets();
}

void SceneTechDemo::build_render_targets()
{
	render_target current_target;
	int i, j, k;
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

				if (z>current_target.face.verticies.at(k).z)
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

	current_target.type = TYPE_ENTITY;
	current_target.the_entity = &spineboy;
	current_target.position = spineboy.position;
	render_targets.push_back(current_target);

	std::sort(render_targets.begin(), render_targets.end(), SceneTechDemo::by_depth_rendertarget());
}

void SceneTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);
	
	// jump
	if (keydown_map[UP] == true)
	{
		//if (spineboy.velocity.y == 0)
			spineboy.velocity.y = +0.035;
	}

	if (spineboy.velocity.y > -0.03)
	{
		spineboy.velocity.y -= 0.0001*time_delta;
	}

	spineboy.correct_against_collisiongroup(collision_group, time_delta);
	spineboy.update(time_delta);
	
	spineboy.spine_data.update_skeleton(time_delta);
	spineboy.player_update(time_delta);

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

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 10;
	LightManager::lights[0].z = -15;
	LightManager::lights[0].radius = 18;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;

}

void SceneTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input]=type;
	spineboy.handle_keypress(input, type);

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void SceneTechDemo::draw()
{
	int i;

	gluLookAt(spineboy.position.x, spineboy.position.y+5, 15, spineboy.position.x, spineboy.position.y, -25, 0, 1, 0);

	// Star Field
	glPushMatrix();
		glTranslatef(spineboy.position.x/1.25, 0.0f, -50.0f);
		for (i = 0; i < myemitter.particles.size(); i++)
		{
			myemitter.particles.at(i)->draw();
		}
	glPopMatrix();

	Paintbrush::draw_collision_group(collision_group, 0);
/*
	t_collisiongroup test;
	test.collision_groups.push_back(spineboy.return_polygon());
	Paintbrush::draw_collision_group(test, 0);
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
	
	BaseTechDemo::draw();
}
