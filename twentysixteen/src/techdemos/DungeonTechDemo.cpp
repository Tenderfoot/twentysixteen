
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
	TechDemoUI.add_widget(new UIImage(0.5, 0.9, 1.01, 0.2, Paintbrush::Soil_Load_Texture("data/images/HUD.png", false, false)));
	TechDemoUI.add_widget(new MapWidget(&grid_manager));
	TechDemoUI.add_widget(new CombatLog(&combat_log));

	combat_log.push_back("Witch took a swing at Mo! [  11 vs 10AC  ]");
	combat_log.push_back("Witch landed a hit for 2 damage!");
	combat_log.push_back("Mo took a swing at Witch!");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");

	grid_manager.init(50, 50);

	lookmode = false;

	grid_manager.entities = &entities;

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
	char_widget = new CharacterWidget(current_char);
	TechDemoUI.add_widget(char_widget);

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
	int i;

	if (lookmode)
	{
		camera_rotation_x += mouse_relative.x / 100;
		camera_rotation_y += mouse_relative.y / 100;

		if (camera_rotation_y > 1.5)
			camera_rotation_y = 1.5;

		if (camera_rotation_y < 0.01)
			camera_rotation_y = 0.01;
	}

	// if turn is over, go to next grid character in entity list;
	int char_index;
	if (current_char->state == GRID_ENDTURN)
	{
		current_char->state = GRID_IDLE;
		current_char->spine_data.animation_name = "idle";

		for (i = 0; i < entities.size(); i++)
		{
			if (current_char == entities.at(i))
			{
				char_index = i;
			}
		}
		current_char = NULL;
		char_index++;
		i = char_index % entities.size();
		while (current_char == NULL)
		{
			if (entities.at(i)->type == GRID_CHARACTER)
			{
				current_char = ((GridCharacter*)entities.at(i));
				char_widget->character = current_char;
				current_char->state = GRID_IDLE;
				grid_manager.compute_visibility_raycast(current_char->position.x, current_char->position.z);
			}
			i++;
			i = i % entities.size();
		}
	}

	// other stuff
	current_char->camera_x_rotation = camera_rotation_x;

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
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
		if (current_char->state != GRID_MOVING || GRID_ATTACKING)
		{
			int entity_pos = grid_manager.entity_on_position(t_vertex(grid_manager.mouse_x, 0.0f, grid_manager.mouse_y));
			if (entity_pos != -1)
			{
				current_char->attack_target((GridCharacter*)entities.at(entity_pos));
			}
			else
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

	// sort and draw entities
	std::vector<Entity*> sort_list;
	sort_list = entities;
	std::sort(sort_list.begin(), sort_list.end(), by_depth_entity());

	int i;
	for (i = 0; i < sort_list.size(); i++)
	{
		glPushMatrix();
			if(grid_manager.tile_map[sort_list.at(i)->position.x][sort_list.at(i)->position.z].discovered)
				sort_list.at(i)->draw();
		glPopMatrix();
	}
}

void DungeonTechDemo::draw_hud()
{
	BaseTechDemo::draw();
}
