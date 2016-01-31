
#include "modeltopolygontechdemo.h"

void ModelToPolygonTechDemo::init()
{
	test_plane = -20;

	TechDemoUI.add_widget(new TextWidget("Collision Tech Demo", 0.5, 0.1, 0.8, 0.15));

	TechDemoUI.add_widget(new TextWidget("W - toggle draw plane", 0.2, 0.4, 0.15, 0.05));
	TechDemoUI.add_widget(new TextWidget("S - toggle draw model", 0.2, 0.5, 0.15, 0.05));

	model_data = ModelData::import("testchamber.fbx", 0.05);

	collision_group = LinearAlgebra::get_collisiongroups_from_model(*model_data, test_plane, t_vertex(0, 0, -50));

	draw_plane = false;
	draw_model = true;
}

void ModelToPolygonTechDemo::run(float time_delta)
{
	rotation += (time_delta / 10);
}

void ModelToPolygonTechDemo::draw()
{
	gluLookAt(sin(rotation / 100) * 3, cos(rotation / 250) * 3, 0, 0, 0, -25, 0, 1, 0);

	glLineWidth(1);

	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, NULL);

	Paintbrush::draw_collision_group(collision_group);

	if (draw_model)
	{
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -50.0f);
			Paintbrush::draw_model(model_data);
		glPopMatrix();
	}

	if (draw_plane)
	{
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, NULL);
			glColor3f(0.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glVertex3f(-100.0f, -100.0f, test_plane - 0.05);
				glVertex3f(100.0f, -100.0f, test_plane - 0.05);
				glVertex3f(100.0f, 100.0f, test_plane - 0.05);
				glVertex3f(-100.0f, 100.0f, test_plane - 0.05);
			glEnd();
		glPopMatrix();
	}

	t_vertex point;
	point.x = cos(((float)SDL_GetTicks()) / 400) * 20;
	point.y = sin(((float)SDL_GetTicks()) / 2000) * 10;

	glPushMatrix();
		glTranslatef(point.x, point.y, test_plane);
		glScalef(0.2f, 0.2f, 0.2f);
		Paintbrush::draw_cube();
	glPopMatrix();

	if (LinearAlgebra::point_in_collisiongroup(point, collision_group))
	{
		printf("collision! %d\n", SDL_GetTicks());
	}
	
	BaseTechDemo::draw();

}

void ModelToPolygonTechDemo::take_input(boundinput input, bool type)
{
	if (type == true)
	{
		if (input == UP)
		{
			draw_plane = !draw_plane;
		}

		if (input == DOWN)
		{
			draw_model = !draw_model;
		}

		if (input == BACK)
		{
			exit_level = TECHDEMO_BASE;
		}
	}

}
