
#include "physicstechdemo.h"

void PhysicsTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Physics Tech Demo", 0.5, 0.15, 0.5, 0.15));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testcollision.fbx", 0.05);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, -20, t_vertex(0, 0, -50));
	spineboy.load_spine_data("everybody");

	box.position = t_vertex(0, 5, -20);
	box.size = t_vertex(1, 3, 1);
	box.velocity = t_vertex(0, 0, 0);
}

void PhysicsTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);
	
	original_pos = box.position;

	if (keydown_map[LEFT] == true)
		box.position.x-=0.01*time_delta;

	if (keydown_map[RIGHT] == true)
		box.position.x += 0.01*time_delta;

	// check x
	if (check_collision(original_pos, box.position))
	{
		box.position.x = original_pos.x;
		box.velocity = t_vertex(0, box.velocity.y, 0);
	}

	box.position.y -= box.velocity.y*time_delta;

	// check y
	if (check_collision(original_pos, box.position))
	{
		box.position.y = original_pos.y;
		box.velocity = t_vertex(box.velocity.x, 0, 0);
	}

	// jump
	if (keydown_map[UP] == true)
	{
		if (box.velocity.y == 0)
			box.velocity.y = -0.035;
	}

	if (box.velocity.y < 0.03)
		box.velocity.y += 0.0001*time_delta;

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
		spineboy.animation_name = "idle";
}

bool PhysicsTechDemo::check_collision(t_vertex previous_position, t_vertex new_position)
{
	// check all 4 points
	bool to_return = false;

	// check all four points of me against the collision group
	t_vertex correction_vertex;

	int i;
	for (i = 0; i < collision_group.collision_groups.size(); i++)
	{
		if (LinearAlgebra::point_in_polygon(t_vertex(new_position.x, new_position.y+2, 0.0f), collision_group.collision_groups.at(i)) || 
			LinearAlgebra::point_in_polygon(t_vertex(new_position.x, new_position.y-2, 0.0f), collision_group.collision_groups.at(i)) ||
			LinearAlgebra::point_in_polygon(t_vertex(new_position.x+1, new_position.y, 0.0f), collision_group.collision_groups.at(i)) ||
			LinearAlgebra::point_in_polygon(t_vertex(new_position.x-1, new_position.y, 0.0f), collision_group.collision_groups.at(i)))
		{
			return true;
		}
	}
	// Then check to make sure none of the collision group is inside me!
	int j, k;
	t_vertex the_vertex;
	for (i = 0; i < collision_group.collision_groups.size(); i++)
	{
		for (j = 0; j < collision_group.collision_groups.at(i).size(); j++)
		{
			for (k = 0; k < collision_group.collision_groups.at(i).at(j).verticies.size(); k++)
			{
				the_vertex = collision_group.collision_groups.at(i).at(j).verticies.at(k);
				if (the_vertex.x > box.position.x - (box.size.x / 2) &&
					the_vertex.x < box.position.x + (box.size.x / 2) &&
					the_vertex.y > box.position.y - (box.size.y / 2) &&
					the_vertex.y < box.position.y + (box.size.y / 2))
				{
					return true;
				}
			}
		}
	}
	
	return to_return;
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

/*
	glPushMatrix();
		glTranslatef(box.position.x, box.position.y, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(box.size.x, box.size.y, 1);
		glBindTexture(GL_TEXTURE_2D, NULL);
		Paintbrush::draw_quad();
	glPopMatrix();
*/

	glPushMatrix();
		glTranslatef(box.position.x, box.position.y-1.9, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(0.006f, 0.006f, 0.006f);
		glRotatef(180 * flip, 0, 1, 0);
		spineboy.draw();
	glPopMatrix();

	BaseTechDemo::draw();
}
