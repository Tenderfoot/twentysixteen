
#include "base_gamelevel.h"

void BaseGameLevel::build_render_targets()
{
	// build grass here - if theres no grass, theres no grass- no harm done.
	int i, j, k;
	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_static.model, t_vertex(0, 0, 0), 0);
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
		render_targets.push_back(current_target);
	}

	printf("rendertargets size: %d\n", render_targets.size());

	std::sort(render_targets.begin(), render_targets.end(), by_depth_rendertarget());
}

void BaseGameLevel::set_camera(t_vertex position, t_vertex lookat)
{
	camera_position = position;
	camera_lookat = lookat;
}

void BaseGameLevel::draw()
{
	gluLookAt(camera_position.x, camera_position.y, camera_position.z, camera_lookat.x, camera_lookat.y, camera_lookat.z, 0, 1, 0);

	//  this line draws the level collision group as lines
	//	Paintbrush::draw_collision_group(collision_group, 0);

	// draw the rendertargets
	glPushMatrix();
	int i;
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

	level_editor.draw();
	glPopMatrix();
}