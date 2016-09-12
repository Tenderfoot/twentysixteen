
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

	lookmode = false;

	camera_rotation_y = 1;
	camera_distance = 25.0f;

	test.spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(test.spine_data.skeleton, "witch");
	test.spine_data.animation_name = "idle";
	test.spine_data.looping = true;
	test.grid_manager = &grid_manager;

	current_char = &test;
	current_char->position.x = 1;
	current_char->position.z = 1;

	// things need to be part of this now!
	entities.push_back(&test);
}

void DungeonTechDemo::run(float time_delta)
{
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

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
	int i, j;
	for (i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(time_delta);
	}
	
	// convert mouse position in space to grid coordinates...
	float x, y;
	x = mouse_in_space.x + 2.5;
	y = mouse_in_space.z + 2.5;
	mouse_relative.x = 0;
	mouse_relative.y = 0;

	x /= 5;
	y /= 5;

	grid_manager.set_mouse_coords(int(x), int(y));
}

void DungeonTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;

	if (input == LMOUSE && type == true)
	{
		current_char->position.x = grid_manager.mouse_x;
		current_char->position.z = grid_manager.mouse_y;
		grid_manager.compute_visibility_raycast(current_char->position.x, current_char->position.z);
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

	grid_manager.x = current_char->position.x;
	grid_manager.y = current_char->position.z;

	gluLookAt((current_char->position.x * 5) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (current_char->position.z * 5) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), current_char->position.x * 5, 0, (current_char->position.z * 5), 0.0f, 1.0f, 0.0f);
	
	grid_manager.draw_3d();

	glPushMatrix();
		test.draw();
	glPopMatrix();
}
