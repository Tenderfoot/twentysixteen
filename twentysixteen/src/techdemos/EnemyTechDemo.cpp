
#include "enemytechdemo.h"

void EnemyTechDemo::init()
{
	level_static.model = ModelData::import("meadow.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	// Point the editor to the entity list

	level_editor.entities = &entities;
	level_editor.render_targets = &render_targets;
	level_editor.read_level("meadow");

	Paintbrush::create_vbo(*level_static.model);

	skeleton.init("skel");
	skeleton.position = t_vertex(10, 30, 0);
	skeleton.size = t_vertex(1, 3, 1);
	skeleton.velocity = t_vertex(0, 0, 0);


	render_target new_entity;
	new_entity.type = TYPE_ENTITY;

	// knight
	new_entity.the_entity = &skeleton;
	new_entity.position = skeleton.position;
	render_targets.push_back(new_entity);

	build_render_targets();
}

void EnemyTechDemo::run(float time_delta)
{
	skeleton.correct_against_collisiongroup(collision_group, time_delta);
	skeleton.update(time_delta);
	skeleton.player_update(time_delta);

	BaseGameLevel::run(time_delta);
}

void EnemyTechDemo::draw()
{
	gluLookAt(camera_position.x, camera_position.y, camera_position.z, camera_lookat.x, camera_lookat.y, camera_lookat.z, 0, 1, 0);

	//  this line draws the level collision group as lines
	Paintbrush::draw_collision_group(collision_group, 0);

	// draw the rendertargets
	glPushMatrix();
	int i;

	Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	Paintbrush::draw_vbo();
	glPopMatrix();
	Paintbrush::stop_shader();

	for (i = 0; i < render_targets.size(); i++)
	{
		if (render_targets.at(i).type == TYPE_FACE)
		{
		
		}
		else
		{
			glPushMatrix();
			render_targets.at(i).the_entity->draw();
			glPopMatrix();
		}
	}

	level_editor.draw();
	glPopMatrix();
}


void EnemyTechDemo::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		LightManager::lights.push_back(Light(0, 0, 0, 100));
	}

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 10;
	LightManager::lights[0].z = 0;
	LightManager::lights[0].radius = 100;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;
}