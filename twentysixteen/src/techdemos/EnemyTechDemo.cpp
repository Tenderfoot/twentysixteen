
#include "enemytechdemo.h"

void EnemyTechDemo::init()
{
	level_static.model = ModelData::import("brandnewscene.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	// Point the editor to the entity list
	level_editor.entities = &entities;
	level_editor.render_targets = &render_targets;
	level_editor.read_level("test");

	knight.init("knight");
	knight.position = t_vertex(10, 30, 0);
	knight.size = t_vertex(1, 3, 1);
	knight.velocity = t_vertex(0, 0, 0);


	render_target new_entity;
	new_entity.type = TYPE_ENTITY;

	// knight
	new_entity.the_entity = &knight;
	new_entity.position = knight.position;
	render_targets.push_back(new_entity);

	build_render_targets();
}

void EnemyTechDemo::run(float time_delta)
{
	knight.correct_against_collisiongroup(collision_group, time_delta);
	knight.update(time_delta);
	knight.player_update(time_delta);

	BaseGameLevel::run(time_delta);
}

