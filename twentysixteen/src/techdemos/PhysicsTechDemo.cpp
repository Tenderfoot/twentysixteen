
#include "physicstechdemo.h"

// K, to build the physics tech demo, you are going to
// -make GameEntity which extends entity
// -make physicsdemo control the gameentity (box)
// -make sure the box can't go in the shapes

void PhysicsTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Physics Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	level_static.model = ModelData::import("testcollision.fbx", 0.05);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, -20, t_vertex(0, 0, -50));

	box.position = t_vertex(0, 5, -20);
	box.size = t_vertex(1, 3, 1);
	box.velocity = t_vertex(0, 0, 0);
}

void PhysicsTechDemo::run(float time_delta)
{
	t_vertex original_pos = box.position;

	if (keydown_map[LEFT] == true)
		box.position.x-=0.01*time_delta;

	if (keydown_map[RIGHT] == true)
		box.position.x += 0.01*time_delta;

	if (keydown_map[UP] == true)
	{
		if (box.velocity.y == 0)
			box.velocity.y = -0.05;
	}

	if (check_collision())
		box.position.x = original_pos.x;

	box.position.y -= box.velocity.y*time_delta;

	if (box.velocity.y < 0.03)
		box.velocity.y += 0.0001*time_delta;

	if (check_collision())
	{
		box.position.y = original_pos.y;
		box.velocity.y = 0;
	}
}

bool PhysicsTechDemo::check_collision()
{
	// check all 4 points
	bool to_return = false;

	if (LinearAlgebra::point_in_collisiongroup(t_vertex(box.position.x - (box.size.x/2), box.position.y - (box.size.y / 2), 0.0f), collision_group))
		to_return = true;

	if (LinearAlgebra::point_in_collisiongroup(t_vertex(box.position.x + (box.size.x / 2), box.position.y - (box.size.y / 2), 0.0f), collision_group))
		to_return = true;

	if (LinearAlgebra::point_in_collisiongroup(t_vertex(box.position.x + (box.size.x / 2), box.position.y + (box.size.y / 2), 0.0f), collision_group))
		to_return = true;

	if (LinearAlgebra::point_in_collisiongroup(t_vertex(box.position.x - (box.size.x / 2), box.position.y + (box.size.y / 2), 0.0f), collision_group))
		to_return = true;

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

	glPushMatrix();
		glTranslatef(box.position.x, box.position.y, -20);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(box.size.x, box.size.y, 1);
		glBindTexture(GL_TEXTURE_2D, NULL);
		Paintbrush::draw_quad();
	glPopMatrix();

	BaseTechDemo::draw();
}
