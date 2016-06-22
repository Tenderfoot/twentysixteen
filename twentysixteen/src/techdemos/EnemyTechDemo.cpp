
#include "enemytechdemo.h"

void EnemyTechDemo::init()
{
	while (myemitter.particles.size() < 1000)
	{
		myemitter.particles.push_back(new Star);
	}

	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(-100, 0, -50), t_vertex(200, 75, 0));

	level_static.model = ModelData::import("brandnewscene.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	spineboy.init();
	spineboy.position = t_vertex(0, 10, 0);
	spineboy.size = t_vertex(1, 3, 1);
	spineboy.velocity = t_vertex(0, 0, 0);

	knight.init("knight");
	knight.position = t_vertex(10, 30, 0);
	knight.size = t_vertex(1, 3, 1);
	knight.velocity = t_vertex(0, 0, 0);


	render_target new_entity;
	new_entity.type = TYPE_ENTITY;

	// spine player
	new_entity.the_entity = &spineboy;
	new_entity.position = spineboy.position;
	render_targets.push_back(new_entity);

	// star emitter
	new_entity.the_entity = &myemitter;
	new_entity.position = myemitter.position;
	render_targets.push_back(new_entity);

	// star background (0.1 gray)
	new_entity.the_entity = new Entity(t_vertex(-100.0f, 0.0f, -55.0f), t_vertex(1000.0f, 1000.0f, 1000.0f), t_vertex(0.1f, 0.1f, 0.1f));
	new_entity.the_entity->texture = NULL;
	new_entity.position = new_entity.the_entity->position;
	render_targets.push_back(new_entity);

	// knight
	new_entity.the_entity = &knight;
	new_entity.position = knight.position;
	render_targets.push_back(new_entity);

	build_render_targets();
}

void EnemyTechDemo::run(float time_delta)
{
	set_camera(t_vertex(spineboy.position.x, spineboy.position.y + 5, 15), t_vertex(spineboy.position.x, spineboy.position.y, -25));

	spineboy.correct_against_collisiongroup(collision_group, time_delta);
	spineboy.update(time_delta);
	spineboy.player_update(time_delta);

	knight.correct_against_collisiongroup(collision_group, time_delta);
	knight.update(time_delta);
	knight.player_update(time_delta);

	myemitter.update(time_delta);

	LightManager::lights[0].y = 5+knight.position.y-camera_position.y;
	LightManager::lights[0].x = knight.position.x-camera_position.x;
}

void EnemyTechDemo::reset()
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
	LightManager::lights[0].radius = 15;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;

}

void EnemyTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input] = type;
	spineboy.handle_keypress(input, type);

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}