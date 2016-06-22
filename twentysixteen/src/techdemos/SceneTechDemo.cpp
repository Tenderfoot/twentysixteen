
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

	spineboy.init();
	spineboy.position = t_vertex(0, 10, 0);
	spineboy.size = t_vertex(1, 3, 1);
	spineboy.velocity = t_vertex(0, 0, 0);

	render_target spine_entity;
	spine_entity.type = TYPE_ENTITY;
	spine_entity.the_entity = &spineboy;
	spine_entity.position = spineboy.position;
	render_targets.push_back(spine_entity);

	build_render_targets();
}

void SceneTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);

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
	LightManager::lights[0].radius = 15;

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

	LightManager::lights[0].y = 5;

	// Star Field
	glPushMatrix();
		glTranslatef(spineboy.position.x / 1.25, 0.0f, -55.0f);
		glScalef(1000.0f, 1000.0f, 1000.0f);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glColor3f(0.1f, 0.1f, 0.1f);
		Paintbrush::draw_quad();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(spineboy.position.x/1.25, 0.0f, -50.0f);
		for (i = 0; i < myemitter.particles.size(); i++)
		{
			myemitter.particles.at(i)->draw();
		}
	glPopMatrix();

//  this line draws the level collision group as lines
//	Paintbrush::draw_collision_group(collision_group, 0);

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
}
