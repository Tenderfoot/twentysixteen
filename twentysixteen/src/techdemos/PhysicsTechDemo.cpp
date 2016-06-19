
#include "physicstechdemo.h"

void PhysicsTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Physics Tech Demo", 0.5, 0.15, 0.5, 0.15));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testcollision4.fbx", 0.05);

	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, -20, t_vertex(0, 0, -50));

	printf("=======this====== %d\n", collision_group.collision_groups.size());

	spineboy.load_spine_data("everybody");

	box.position = t_vertex(0, 10, -20);
	box.size = t_vertex(1, 2, 1);
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
		if (box.velocity.y == 0)
			box.velocity.y = +0.035;
	}

	if (box.velocity.y > -0.03)
		box.velocity.y -= 0.0001*time_delta;

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

	check_collision(time_delta);
}

bool PhysicsTechDemo::check_collision(float time_delta)
{
	// check all 4 points
	bool to_return = false;

	// check all four points of me against the collision group
	t_vertex correction_vertex;

	t_vertex polygonATranslation = t_vertex(0, 0, 0);
	t_vertex real_velocity = t_vertex(0, 0, 0.0f);

	bool intersected = false;

	int i,j;
	for (i = 0; i < collision_group.collision_groups.size(); i++)
	{
		PolygonCollisionResult r = LinearAlgebra::PolygonCollision(box.return_polygon(), collision_group.collision_groups.at(i), real_velocity);

		if (r.WillIntersect && intersected == false) {
			// Move the polygon by its velocity, then move
			// the polygons appart using the Minimum Translation Vector

			box.position.x += real_velocity.x + r.MinimumTranslationVector.x*1.05;
			box.position.y += real_velocity.y + r.MinimumTranslationVector.y*1.05;

			if(r.MinimumTranslationVector.y > 0)
				box.velocity.y = 0;

			if (r.MinimumTranslationVector.y < 0)
				box.velocity.y = -0.001;

			intersected = true;
		}
	}

	if (intersected == false)
	{
		box.position.x += (box.velocity.x*time_delta);
		box.position.y += (box.velocity.y*time_delta);
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
	

	Paintbrush::draw_collision_group(collision_group, -20);

	t_collisiongroup testbox;
	testbox.collision_groups.push_back(box.return_polygon());
	Paintbrush::draw_collision_group(testbox, -20);

/*
	glPushMatrix();
		glTranslatef(box.position.x, box.position.y, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(box.size.x, box.size.y, 1);
		glBindTexture(GL_TEXTURE_2D, NULL);
		Paintbrush::draw_quad();
	glPopMatrix();
*/
	/*
	glPushMatrix();
		glTranslatef(box.position.x, box.position.y, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(box.size.x, box.size.y, 0.006f);
		Paintbrush::draw_quad();
	glPopMatrix();
	*/

	BaseTechDemo::draw();
}
