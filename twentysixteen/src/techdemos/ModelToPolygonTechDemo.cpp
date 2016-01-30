
#include "modeltopolygontechdemo.h"

void ModelToPolygonTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Model To Polygon Tech Demo", 0.5, 0.1, 0.8, 0.15));

	model_data = ModelData::import("testchamber.fbx", 0.05);

	draw_plane = true;
	draw_model = true;
}

void ModelToPolygonTechDemo::run(float time_delta)
{
}

void ModelToPolygonTechDemo::draw()
{
	BaseTechDemo::draw();

	float test_plane = -20;

	int i;
	std::vector<t_edge> *edge_set = LinearAlgebra::get_edges(*model_data);

	glLineWidth(1);

	std::vector<t_edge>::iterator it;

	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, NULL);

	for (auto it = edge_set->begin(); it != edge_set->end(); ++it)
	{
		glColor3f(1.0f, 0.f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(it->v1.x, it->v1.y, test_plane);
		glVertex3f(it->v2.x, it->v2.y, test_plane);
		glEnd();

		glPushMatrix();
		glTranslatef(it->v1.x, it->v1.y, test_plane);
		glScalef(0.1f, 0.1f, 0.1f);
		Paintbrush::draw_cube();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(it->v2.x, it->v2.y, test_plane);
		glScalef(0.1f, 0.1f, 0.1f);
		Paintbrush::draw_cube();
		glPopMatrix();
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

	delete edge_set;

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
	}

}
