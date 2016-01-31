
#include "modeltopolygontechdemo.h"

void ModelToPolygonTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Model To Polygon Tech Demo", 0.5, 0.1, 0.8, 0.15));

	model_data = ModelData::import("testchamber.fbx", 0.05);
	edge_set = LinearAlgebra::get_edges_from_plane(*model_data);

	draw_plane = true;
	draw_model = true;
}

void ModelToPolygonTechDemo::run(float time_delta)
{
}

void ModelToPolygonTechDemo::draw()
{
	float test_plane = -20;

	int i;

	glLineWidth(1);

	std::vector<t_edge>::iterator it;

	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, NULL);

	for (auto it = edge_set->begin(); it != edge_set->end(); ++it)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(it->verticies.at(0).x, it->verticies.at(0).y, test_plane);
		glVertex3f(it->verticies.at(1).x, it->verticies.at(1).y, test_plane);
		glEnd();
	}

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
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glVertex3f(-100.0f, -100.0f, test_plane - 0.05);
				glVertex3f(100.0f, -100.0f, test_plane - 0.05);
				glVertex3f(100.0f, 100.0f, test_plane - 0.05);
				glVertex3f(-100.0f, 100.0f, test_plane - 0.05);
			glEnd();
		glPopMatrix();
	}

	t_vertex point;
	point.x = cos(((float)SDL_GetTicks()) / 1000) * 20;
	point.y = sin(((float)SDL_GetTicks()) / 1000) * 20;

	glPushMatrix();
		glTranslatef(point.x, point.y, test_plane);
		glScalef(0.2f, 0.2f, 0.2f);
		Paintbrush::draw_cube();
	glPopMatrix();

	if (LinearAlgebra::point_in_polygon(point, *edge_set))
	{
		printf("collision!\n");
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
