
#include "physicstechdemo.h"

void PhysicsTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Physics Tech Demo", 0.5, 0.15, 0.5, 0.15));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("data/models/testcollision4.fbx", 0.05);

	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, -20, t_vertex(0, 0, -50));

	spineboy.load_spine_data("everybody");
	spSkeleton_setSkinByName(spineboy.skeleton, "mo");
	spineboy.animation_name = "idle";

	box.position = t_vertex(0, 6, -20);
	box.size = t_vertex(1, 3, 1);
	box.velocity = t_vertex(0, 0, 0);
}

void PhysicsTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta); 
	
	original_pos = box.position;

	if (keydown_map[LEFT] == true)
		box.velocity.x = -0.01;
	else if (keydown_map[RIGHT] == true)
		box.velocity.x = 0.01;
	else
		box.velocity.x = 0.0; 

	// jump
	if (keydown_map[UP] == true)
	{
		box.velocity.y = +0.035;
	}
	
	if (box.velocity.y > -0.03)
	{
		box.velocity.y -= 0.0001*time_delta;
	}

	if (keydown_map[LEFT] || keydown_map[RIGHT])
	{
		spineboy.animation_name = "walk_two";

		if (keydown_map[LEFT])
		{
			flip = false;
		}

		if (keydown_map[RIGHT])
		{
			flip = true;
		}
	}
	else
	{
		spineboy.animation_name = "idle";
	}

	check_collision(time_delta);
}

bool PhysicsTechDemo::check_collision(float time_delta)
{
	box.correct_against_collisiongroup(collision_group, time_delta);
	box.update(time_delta);

	return true;
}

void PhysicsTechDemo::take_input(boundinput input, bool type)
{
	keydown_map[input] = type;

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void PhysicsTechDemo::draw()
{
	gluLookAt(box.position.x, box.position.y, 0, box.position.x, box.position.y, -25, 0, 1, 0);
	
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -50.0f);
		Paintbrush::use_shader(level_static.shader);
		Paintbrush::draw_model(level_static.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	Paintbrush::draw_collision_group(collision_group, -20);

	glPushMatrix();
		glTranslatef(box.position.x, box.position.y-1.5, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		if(flip)
			glRotatef(180, 0, 1, 0);
		glScalef(0.005, 0.005, 1);
		spineboy.draw();
	glPopMatrix();

	BaseTechDemo::draw();
}
