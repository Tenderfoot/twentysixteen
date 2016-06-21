
#include "grasstechdemo.h"

void GrassTechDemo::init()
{
	level_static.model = ModelData::import("brandnewscene.fbx", 0.005);

	// build grass
	int i;
	std::vector<Entity*> grass_entities = VFXGrass::generate_grass(*level_static.model, t_vertex(0, 0, 0), 0);
	for (i = 0; i < grass_entities.size(); i++)
	{
		entities.push_back(grass_entities.at(i));
	}

	build_render_targets();
}

void GrassTechDemo::build_render_targets()
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


	std::sort(render_targets.begin(), render_targets.end(), GrassTechDemo::by_depth_rendertarget());
}

void GrassTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);

}

void GrassTechDemo::reset()
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
	LightManager::lights[0].radius = 25;

	LightManager::lights[0].r = 0.9;
	LightManager::lights[0].g = 0.9;
	LightManager::lights[0].b = 0.9;

}

void GrassTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input] = type;

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void GrassTechDemo::draw()
{
	int i;

	gluLookAt(0, 20, 15,0, 10, -25, 0, 1, 0);

	LightManager::lights[0].y = 10;

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
