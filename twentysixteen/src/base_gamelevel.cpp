
#include "base_gamelevel.h"

void BaseGameLevel::build_render_targets()
{
	// build grass here - if theres no grass, theres no grass- no harm done.
	int i, j, k;
	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_data, t_vertex(0, 0, 0), 0);
	for (i = 0; i < grass_entities.size(); i++)
	{
		entities.push_back(grass_entities.at(i));
	}

	// loop through and populate rendertargets
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

				if (z>current_target.face.verticies.at(k).z)
					z = current_target.face.verticies.at(k).z;
			}
			x = x / current_target.face.verticies.size();
			y = y / current_target.face.verticies.size();

			current_target.position = t_vertex(x, y, z);

			render_targets.push_back(current_target);
		}
	}

	for (i = 0; i < entities.size(); i++)
	{
		current_target.type = TYPE_ENTITY;
		current_target.the_entity = entities.at(i);
		current_target.position = entities.at(i)->position;

		// this is a dirty way to do this
		// give every entity access to the entities and render targets
		// so they can add entites to the game later
		entities.at(i)->game_entities = &entities;

		render_targets.push_back(current_target);
	}

	num_entities = entities.size();
	std::sort(render_targets.begin(), render_targets.end(), by_depth_rendertarget());
}

void BaseGameLevel::set_camera(t_vertex position, t_vertex lookat)
{
	camera_position = position;
	camera_lookat = lookat;
}

void BaseGameLevel::run(float time_delta)
{

	// See if entities has increased
	if (num_entities < entities.size())
	{
		int extra_entities = entities.size() - num_entities;
		render_target current_target;
		int i;
		for (i = 0; i < extra_entities; i++)
		{
			current_target.type = TYPE_ENTITY;
			current_target.the_entity = entities.at(entities.size() - 1 - i);
			current_target.position = entities.at(entities.size() - 1 - i)->position;
			render_targets.push_back(current_target);
			// this is a dirty way to do this
			// give every entity access to the entities and render targets
			// so they can add entites to the game later
			entities.at(entities.size() - 1)->game_entities = &entities;

			num_entities = entities.size();
		}
		std::sort(render_targets.begin(), render_targets.end(), by_depth_rendertarget());
	}
	else if (num_entities > entities.size())
		num_entities = entities.size();

	if (level_editor.editor_mode == CREATE_MODE)
	{
		set_camera(t_vertex(level_editor.camera_position.x, level_editor.camera_position.y + 5, 15), t_vertex(level_editor.camera_position.x, level_editor.camera_position.y, -25));
	}
	else if (level_editor.editor_mode == EDIT_MODE)
	{
		set_camera(t_vertex(level_editor.camera_position.x, level_editor.camera_position.y + 5, 15), t_vertex(level_editor.camera_position.x, level_editor.camera_position.y, -25));
	}
	else
	{
		t_collisiongroup test;
		int i, j;

		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->type == SKELETON_ENTITY || entities.at(i)->type == PORTCULLIS_ENTITY || entities.at(i)->type == GAME_ENTITY)
			{
				test.collision_groups.push_back(((GameEntity*)entities.at(i))->return_polygon());
			}
		}

		for (i = 0; i < collision_group.collision_groups.size(); i++)
		{
			test.collision_groups.push_back(collision_group.collision_groups.at(i));
		}

		for (i = 0; i < entities.size(); i++)
		{
			if (entities.at(i)->type == GAME_ENTITY)
			{
				((GameEntity*)entities.at(i))->correct_against_collisiongroup(collision_group, time_delta);
				((GameEntity*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == ARROW_ENTITY)
			{
				((ArrowEntity*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == PORTCULLIS_ENTITY)
			{
				((PortcullisEntity*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == BUTTON_ENTITY)
			{
				((ButtonEntity*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == SKELETON_ENTITY)
			{
				((SkeletonEntity*)entities.at(i))->correct_against_collisiongroup(collision_group, time_delta);
				((SkeletonEntity*)entities.at(i))->update(time_delta);
				((SkeletonEntity*)entities.at(i))->player_update(time_delta);
			}
			if (entities.at(i)->type == PLAYER_ENTITY)
			{
				((PlayerEntity*)entities.at(i))->correct_against_collisiongroup(test, time_delta);
				((PlayerEntity*)entities.at(i))->update(time_delta);
				((PlayerEntity*)entities.at(i))->player_update(time_delta);
				set_camera(t_vertex(((PlayerEntity*)entities.at(i))->position.x, ((PlayerEntity*)entities.at(i))->position.y + 10, 20), t_vertex(((PlayerEntity*)entities.at(i))->position.x, ((PlayerEntity*)entities.at(i))->position.y, -25));
			}
			if (entities.at(i)->type == EMITTER_ENTITY)
			{
				((ParticleEmitter*)entities.at(i))->update(time_delta);
			}
			if (entities.at(i)->type == ARCHER_ENTITY)
			{
				((ArcherEntity*)entities.at(i))->correct_against_collisiongroup(test, time_delta);
				((ArcherEntity*)entities.at(i))->update(time_delta);
				((ArcherEntity*)entities.at(i))->player_update(time_delta);
			}
		}
	}

	level_editor.update();
}

void BaseGameLevel::draw()
{
	gluLookAt(camera_position.x, camera_position.y, camera_position.z, camera_lookat.x, camera_lookat.y, camera_lookat.z, 0, 1, 0);

	//  this line draws the level collision group as lines
//	Paintbrush::draw_collision_group(collision_group, 0);

	// draw the rendertargets
	glPushMatrix();
	int i;

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		Paintbrush::draw_vbo(level_vbo);
	glPopMatrix();
	Paintbrush::stop_shader();


	for (i = 0; i < render_targets.size(); i++)
	{
		if (render_targets.at(i).type != TYPE_FACE)
		{
			glPushMatrix();
				render_targets.at(i).the_entity->draw();
			glPopMatrix();
		}
	}

	level_editor.draw();
	glPopMatrix();
}

void BaseGameLevel::take_input(boundinput input, bool type)
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

void BaseGameLevel::init_level(std::string level_name)
{
	level_editor.editor_mode = PLAY_MODE;

	level_static.model = ModelData::import("data/levels/"+level_name + "/" + level_name + ".fbx", 0.005);
	level_data = ModelData::import("data/levels/" + level_name + "/" + level_name + "_data.fbx", 0.005);

	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_data, 0, t_vertex(0, 0, 0));

	// Point the editor to the entity list
	level_editor.entities = &entities;
	level_editor.render_targets = &render_targets;
	level_editor.read_level(level_name);

	level_vbo = Paintbrush::create_vbo(*level_static.model);
	level_vbo.texture = Paintbrush::get_texture("data/levels/"+level_name+"/"+level_name+"_atlas.png", false, true);
}

void BaseGameLevel::reset()
{
	LightManager::reset();
	while (LightManager::lights.size() < 1)
	{
		LightManager::lights.push_back(Light(0, 0, 0, 100));
	}

	LightManager::lights[0].x = 0;
	LightManager::lights[0].y = 10;
	LightManager::lights[0].z = -20;
	LightManager::lights[0].radius = 35;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;
}
