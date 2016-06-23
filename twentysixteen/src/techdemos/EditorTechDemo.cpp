
#include "editortechdemo.h"

void EditorTechDemo::init()
{
	while (myemitter.particles.size() < 1000)
	{
		myemitter.particles.push_back(new Star);
	}

	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(-100, 0, -50), t_vertex(200, 75, 0));

	level_static.model = ModelData::import("brandnewscene.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	entities.push_back(&myemitter);

	// Point the editor to the entity list
	level_editor.entities = &entities;
	level_editor.render_targets = &render_targets;
	level_editor.current_entity = 0;
	level_editor.read_level();

	// add the player and emitter to rendertargets
	render_target new_entity;
	new_entity.type = TYPE_ENTITY;

	// star emitter
	new_entity.the_entity = &myemitter;
	new_entity.position = myemitter.position;
	render_targets.push_back(new_entity);

	// star background (0.1 gray)
	new_entity.the_entity = new Entity(t_vertex(-100.0f, 0.0f, -55.0f), t_vertex(1000.0f, 1000.0f, 1000.0f), t_vertex(0.1f, 0.1f, 0.1f));
	new_entity.the_entity->texture = NULL;
	new_entity.position = new_entity.the_entity->position;
	render_targets.push_back(new_entity);

	build_render_targets();
}

void EditorTechDemo::run(float time_delta)
{
	if (level_editor.editor_mode == CREATE_MODE)
	{
		set_camera(t_vertex(level_editor.camera_position.x, level_editor.camera_position.y + 5, 15), t_vertex(level_editor.camera_position.x, level_editor.camera_position.y, -25));
	}
	else if(level_editor.editor_mode == EDIT_MODE)
	{
		set_camera(t_vertex(level_editor.camera_position.x, level_editor.camera_position.y + 5, 15), t_vertex(level_editor.camera_position.x, level_editor.camera_position.y, -25));
	}
	else
	{
		int i;
		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->type == GAME_ENTITY)
			{
				((GameEntity*)entities.at(i))->correct_against_collisiongroup(collision_group, time_delta);
				((GameEntity*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == PLAYER_ENTITY)
			{
				set_camera(t_vertex(((PlayerEntity*)entities.at(i))->position.x, ((PlayerEntity*)entities.at(i))->position.y + 5, 15), t_vertex(((PlayerEntity*)entities.at(i))->position.x, ((PlayerEntity*)entities.at(i))->position.y, -25));
				((PlayerEntity*)entities.at(i))->player_update(time_delta);
				((PlayerEntity*)entities.at(i))->correct_against_collisiongroup(collision_group, time_delta);
				((PlayerEntity*)entities.at(i))->update(time_delta);
			}
		}
	}

	myemitter.update(time_delta);
	level_editor.update();

	LightManager::lights[0].y = 5;
}

void EditorTechDemo::reset()
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

void EditorTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input] = type;
	level_editor.take_input(input, type);
	
	if (level_editor.editor_mode == PLAY_MODE)
	{
		int i;
		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->type == PLAYER_ENTITY)
			{
				((PlayerEntity*)entities.at(i))->handle_keypress(input, type);
			}
		}
	}

	if (input == EDITOR_PLAY_MODE && type == true)
	{
		std::sort(render_targets.begin(), render_targets.end(), by_depth_rendertarget());
	}

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}