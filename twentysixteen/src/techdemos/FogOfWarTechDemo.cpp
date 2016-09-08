
#include "fogofwartechdemo.h"

void FogOfWarTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Fog Of War", 0.5, 0.1, 0.5, 0.15));

	int i, j;

	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			tile_map[i][j] = 0;

}

void FogOfWarTechDemo::run(float time_delta)
{
}

void FogOfWarTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = QUIT;
}

void FogOfWarTechDemo::draw()
{
	BaseTechDemo::draw();

	int i, j;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
		{
			glPushMatrix();
			glTranslatef(i-5, j-5, -10.0f);
			glDisable(GL_TEXTURE_2D);
			glColor3f(((float)i) / 10, ((float)j) / 10, 1.0f);
			Paintbrush::draw_quad();
			glPopMatrix();
			glEnable(GL_TEXTURE_2D);
			tile_map[i][j] = 0;
		}
}
