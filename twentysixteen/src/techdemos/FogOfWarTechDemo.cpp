
#include "fogofwartechdemo.h"
#include "../game_entity.h"

void FogOfWarTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Fog Of War", 0.5, 0.1, 0.5, 0.15));
	grid_manager.init(50,50);

	spineboy.load_spine_data("everybody");
	spSkeleton_setSkinByName(spineboy.skeleton, "mo");
	spineboy.animation_name = "idle";
	spineboy.looping = true;
}

void FogOfWarTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);
}

void FogOfWarTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = QUIT;

	if (input == RIGHT && type == true)
	{
		if (x < grid_manager.width-1 && grid_manager.tile_map[x+1][y].wall != 1)
		{
			x++;
			grid_manager.compute_visibility_raycast(x, y);
		}
	}

	if (input == LEFT && type == true)
	{
		if (x > 0 && grid_manager.tile_map[x - 1][y].wall != 1)
		{
			x--;
			grid_manager.compute_visibility_raycast(x, y);
		}
	}

	if (input == UP && type == true)
	{
		if (y > 0 && grid_manager.tile_map[x][y - 1].wall != 1)
		{
			y--;
			grid_manager.compute_visibility_raycast(x, y);
		}
	}

	if (input == DOWN && type == true)
	{
		if (y < grid_manager.height - 1 && grid_manager.tile_map[x][y + 1].wall != 1)
		{
			y++;
			grid_manager.compute_visibility_raycast(x, y);
		}
	}
}

void FogOfWarTechDemo::draw()
{
	BaseTechDemo::draw();

	grid_manager.x = x;
	grid_manager.y = y;

	gluLookAt((x*5), +50, (y*5)+15.0f, x*5, 0, (y * 5), 0.0f, 1.0f, 0.0f);
	//grid_manager.draw_2d();
	grid_manager.draw_3d();

	glPushMatrix();
		glTranslatef(x*5, 0, y*5);
		glScalef(0.01f, 0.01f, 0.01f);
		spineboy.draw();
	glPopMatrix();

}
