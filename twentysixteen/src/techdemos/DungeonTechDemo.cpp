
#include "dungeontechdemo.h"
#include "../game_entity.h"

// going to make a grid character thats like an entity
// at any one point in time, its a characters turn
// that character can enter several states, for example, "moving"
// when a character has exhausted its actions it is the next characters turn

t_vertex Level::camera_position;
t_vertex Level::camera_lookat;

void DungeonTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Dungeon Tech Demo", 0.5, 0.1, 0.5, 0.15));
	grid_manager.init(50, 50);

	lookmode = false;

	camera_rotation_y = 1;
	camera_distance = 25.0f;

	test = new GridCharacter();
	test->spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(test->spine_data.skeleton, "witch");
	test->spine_data.animation_name = "idle";
	test->spine_data.looping = true;
	test->grid_manager = &grid_manager;
	entities.push_back(test);
	
	current_char = test;

	test = new GridCharacter();
	test->spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(test->spine_data.skeleton, "mo");
	test->spine_data.animation_name = "idle";
	test->spine_data.looping = true;
	test->grid_manager = &grid_manager;
	test->position = t_vertex(5, 0, 5);
	entities.push_back(test);

	current_char->position.x = 1;
	current_char->position.z = 1;
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

	current_char->camera_x_rotation = camera_rotation_x;

	std::sort(entities.begin(), entities.end(), by_depth_entity());

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
	int i, j;
	for (i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(time_delta);
		((GridCharacter*)entities.at(i))->camera_x_rotation = camera_rotation_x;
	}
	
	// convert mouse position in space to grid coordinates...
	if (current_char->state != GRID_MOVING)
	{
		float x, y;
		x = mouse_in_space.x + 2.5;
		y = mouse_in_space.z + 2.5;

		x /= 5;
		y /= 5;

		grid_manager.set_mouse_coords(int(x), int(y));
	}

	mouse_relative.x = 0;
	mouse_relative.y = 0;

	// this sets the current position right now
	// will be unnecissary when entities are using
	// grid manager
	grid_manager.x = current_char->position.x;
	grid_manager.y = current_char->position.z;
}

void DungeonTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;

	if (input == LMOUSE && type == true)
	{
		if (current_char->state != GRID_MOVING)
		{
			current_char->set_moving(t_vertex(grid_manager.mouse_x, 0.0f, grid_manager.mouse_y));
		}
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

	t_vertex camera_pos;
	if (current_char->state == GRID_MOVING)
		camera_pos = current_char->draw_position;
	else
		camera_pos = current_char->position;

	// this stuff is for the current draw order
	camera_position = t_vertex((camera_pos.x) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y));
	camera_lookat = t_vertex(camera_pos.x, 0, (camera_pos.z));

	gluLookAt((camera_pos.x * 5) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z * 5) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_pos.x * 5, 0, (camera_pos.z * 5), 0.0f, 1.0f, 0.0f);
	grid_manager.draw_3d();

	int i, j;
	for (i = 0; i < entities.size(); i++)
	{
		glPushMatrix();
			if(grid_manager.tile_map[entities.at(i)->position.x][entities.at(i)->position.z].discovered)
				entities.at(i)->draw();
		glPopMatrix();
	}
}
