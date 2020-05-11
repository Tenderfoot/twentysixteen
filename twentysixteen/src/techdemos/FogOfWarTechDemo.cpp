#include "fogofwartechdemo.h"
#include "../game_entity.h"
#include "../grid_abilities.h"


// TODO:

// RTS Stuff
	// FOWPlayerController
	// Greenboxing / Unit selection

// TILE STUFF:
	// add dark grass
	// add trees
		// fix
	// split vision blockage between water and rocks
	// paint terrain (edit mode for grid stuff??)


void FogOfWarTechDemo::init()
{
	Ability_Manager::build_abilities();
	Ability_Manager::grid_manager = &grid_manager;

	TechDemoUI.add_widget(new UIImage(0.5, 0.9, 1.01, 0.2, Paintbrush::Soil_Load_Texture("data/images/HUD.png", false, false)));
	TechDemoUI.add_widget(new MapWidget(&grid_manager));

	green_box = new GreenBox();
	TechDemoUI.add_widget(green_box);

	grid_manager.entities = &entities;
	grid_manager.init();

	// make the player
	new_player = new FOWPlayer();

	// This will spawn the character for now
	character_manager.grid_manager = &grid_manager;

	for (int i = 0; i < entities.size(); i++)
	{
		Entity *current_entity = entities.at(i);
		if (current_entity->type == GRID_SPAWNPOINT)
		{
			new_character = new FOWCharacter();
			new_character->spine_data.load_spine_data("everybody");
			spSkeleton_setSkinByName(new_character->spine_data.skeleton, "witch");
			new_character->spine_data.animation_name = "idle";
			new_character->spine_data.looping = true;
			new_character->grid_manager = &grid_manager;
			new_character->position = current_entity->position;
			entities.push_back(new_character);
		}
	}

	lookmode = false;

	camera_rotation_y = 1;
	camera_distance = 25.0f;

	// This adds the character widget to the UI
	// will need to be updated to use the players selected unit
	GridCharacter *current_char = new_character;
	char_widget = new CharacterWidget(new_character);
	TechDemoUI.add_widget(char_widget);
	grid_manager.compute_visibility_raycast(current_char->position.x, current_char->position.z, true);

	reset();
}

void FogOfWarTechDemo::run(float time_delta)
{
	grid_manager.lookmode = lookmode;

	//character_manager.run(mouse_in_space, camera_rotation_x);
	//char_widget->character = character_manager.get_current_character();

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
	for (int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(time_delta);
	}
}

void FogOfWarTechDemo::take_input(boundinput input, bool type)
{
	GridCharacter *current_char = new_character;

	if (input == MOUSEMOTION)
	{
		TechDemoUI.mouse_coords = t_vertex(mousex, mousey, 0);
		TechDemoUI.mouse_focus();

		if (lookmode)
		{
			camera_rotation_x += mouse_relative.x / 100;
			camera_rotation_y += mouse_relative.y / 100;
			mouse_relative.x = 0;
			mouse_relative.y = 0;

			if (camera_rotation_y > 1.5)
				camera_rotation_y = 1.5;

			if (camera_rotation_y < 0.01)
				camera_rotation_y = 0.01;
		}
	}

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;

	if (input == LMOUSE && type == true)
	{
		if (TechDemoUI.mouse_focus() != -1)
		{
			if (TechDemoUI.widgets.at(TechDemoUI.mouse_focus())->absorbs_mouse)
			{
				TechDemoUI.widgets.at(TechDemoUI.mouse_focus())->click_at_location(t_vertex(mousex, mousey, 0));
				current_char->active_ability = ability_bar->get_active();
			}
		}
		else
		{
			Ability_Manager::use_ability(current_char, mouse_in_space);
		}

		green_box->x = mousex;
		green_box->y = mousey;
		green_box->visible = true;
	}

	if (input == LMOUSE && type == false)
	{
		green_box->visible = false;
	}

	if (input == RIGHT && type == true)
	{
		camera_pos.x++;
	}

	if (input == LEFT && type == true)
	{
		camera_pos.x--;
	}

	if (input == UP && type == true)
	{
		camera_pos.z--;
	}

	if (input == DOWN && type == true)
	{
		camera_pos.z++;
	}

	if (input == EDITOR_T && type == true)
	{
		grid_manager.use_tex = !grid_manager.use_tex;
	}

	if (input == NEXT && type == true)
	{
		grid_manager.randomize_map();
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

void FogOfWarTechDemo::draw()
{
	GridCharacter *current_char = new_character;

	camera_rotation_y = 0.5;
	camera_rotation_x = 0;

	// this stuff is for the current draw order
	camera_position = t_vertex((camera_pos.x) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y));
	camera_lookat = t_vertex(camera_pos.x, 0, (camera_pos.z));

	gluLookAt((camera_pos.x * 5), camera_distance, (camera_pos.z * 5), camera_pos.x * 5, 0, (camera_pos.z * 5)-0.000001, 0.0f, 1.0f, 0.0f);

	grid_manager.draw_autotile();

	// green box
	green_box->width = mousex;
	green_box->height = mousey;


	// sort and draw entities
	std::vector<Entity*> sort_list;
	sort_list = entities;
	std::sort(sort_list.begin(), sort_list.end(), by_depth_entity());

	int i;
	for (i = 0; i < sort_list.size(); i++)
	{
		glPushMatrix();
			sort_list.at(i)->draw();
		glPopMatrix();
	}
}

void FogOfWarTechDemo::reset()
{
	LightManager::reset();

	while (LightManager::lights.size() < 1)
	{
		// make sure there are actually two lights to manipulate
		LightManager::lights.push_back(Light(0, 0, 0, 20));
	}

	// make this motherfucker red
	LightManager::lights[0].radius = 20;
	LightManager::lights[0].r = 1;
	LightManager::lights[0].g = 1;
	LightManager::lights[0].b = 1;
}

void FogOfWarTechDemo::draw_hud()
{
	BaseTechDemo::draw();
}
