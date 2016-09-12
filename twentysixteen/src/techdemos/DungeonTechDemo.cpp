
#include "dungeontechdemo.h"
#include "../game_entity.h"

// going to make a grid character thats like an entity
// at any one point in time, its a characters turn
// that character can enter several states, for example, "moving"
// when a character has exhausted its actions it is the next characters turn

void DungeonTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Dungeon Tech Demo", 0.5, 0.1, 0.5, 0.15));
	grid_manager.init(50, 50);

	spineboy.load_spine_data("everybody");
	spSkeleton_setSkinByName(spineboy.skeleton, "mo");
	spineboy.animation_name = "idle";
	spineboy.looping = true;

	lookmode = false;

	camera_rotation_y = 1;
	camera_distance = 25.0f;

	x = 1;
	y = 1;
}

void DungeonTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);

	grid_manager.lookmode = lookmode;

	if (lookmode)
	{
		camera_rotation_x += mouse_relative.x / 100;
		camera_rotation_y += mouse_relative.y / 100;

		if (camera_rotation_y > 1.5)
			camera_rotation_y = 1.5;

		if (camera_rotation_y < 0.01)
			camera_rotation_y = 0.01;
	}

	// This is because they are only set when there is deltas
	mouse_relative.x = 0;
	mouse_relative.y = 0;

	// convert mouse position in space to grid coordinates...
	float x, y;
	x = mouse_in_space.x + 2.5;
	y = mouse_in_space.z + 2.5;

	x /= 5;
	y /= 5;

	grid_manager.set_mouse_coords(int(x), int(y));
}

void DungeonTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;

	if (input == RIGHT && type == true)
	{
	}

	if (input == LEFT && type == true)
	{
	}

	if (input == UP && type == true)
	{

	}

	if (input == DOWN && type == true)
	{
	}

	if (input == LMOUSE && type == true)
	{
		x = grid_manager.mouse_x;
		y = grid_manager.mouse_y;

		grid_manager.compute_visibility_raycast(x, y);
	}

	if (input == RMOUSE && type == true)
	{
		lookmode = true;
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	if (input == RMOUSE && type == false)
	{
		lookmode = false;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (input == MWHEELUP)
	{
		if (camera_distance > 5)
			camera_distance--;
	}

	if (input == MWHEELDOWN)
	{
		if (camera_distance < 100)
			camera_distance++;
	}

	if (input == MIDDLEMOUSE)
	{
		camera_distance = 25.0f;
		camera_rotation_y = 1;
		camera_rotation_x = 0;
	}

}

void DungeonTechDemo::draw()
{
	BaseTechDemo::draw();

	grid_manager.x = x;
	grid_manager.y = y;

	gluLookAt((x * 5) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (y * 5) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), x * 5, 0, (y * 5), 0.0f, 1.0f, 0.0f);
	//	grid_manager.draw_2d();
	grid_manager.draw_3d();

	glPushMatrix();
	glTranslatef(x * 5, 0, y * 5);
	glRotatef(camera_rotation_x*57.2958, 0.0f, 1.0f, 0.0f);
	glScalef(0.01f, 0.01f, 0.01f);
	spineboy.draw();
	glPopMatrix();

	/*	3d cursor
	if (!lookmode)
	{
	glPushMatrix();
	glTranslatef(mouse_in_space.x, 0, mouse_in_space.z);
	Paintbrush::draw_cube();
	glPopMatrix();
	}
	*/

}
