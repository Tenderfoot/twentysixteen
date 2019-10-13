
#include "dungeontechdemo.h"
#include "../game_entity.h"
#include "../grid_abilities.h"

t_vertex Level::camera_position;
t_vertex Level::camera_lookat;

void DungeonTechDemo::init()
{
	Ability_Manager::build_abilities();
	Ability_Manager::grid_manager = &grid_manager;

	TechDemoUI.add_widget(new UIImage(0.5, 0.9, 1.01, 0.2, Paintbrush::Soil_Load_Texture("data/images/HUD.png", false, false)));
	TechDemoUI.add_widget(new MapWidget(&grid_manager));
	TechDemoUI.add_widget(new CombatLog(&combat_log));
	ability_bar = new AbilityBar();
	TechDemoUI.add_widget(ability_bar);

	combat_log.push_back("Witch took a swing at Mo! [  11 vs 10AC  ]");
	combat_log.push_back("Witch landed a hit for 2 damage!");
	combat_log.push_back("Mo took a swing at Witch!");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");
	combat_log.push_back("Mo missed! [  8 vs 10AC  ]");

	grid_manager.entities = &entities;
	grid_manager.init();
	character_manager.grid_manager = &grid_manager;
	character_manager.spawn_characters(&entities);

	lookmode = false;

	camera_rotation_y = 1;
	camera_distance = 25.0f;

	// Some other stuff
	GridCharacter *current_char = character_manager.get_current_character();
	char_widget = new CharacterWidget(current_char);
	TechDemoUI.add_widget(char_widget);
	grid_manager.compute_visibility_raycast(current_char->position.x, current_char->position.z, true);

	reset();
}

void DungeonTechDemo::run(float time_delta)
{
	grid_manager.lookmode = lookmode;
	character_manager.run(mouse_in_space, camera_rotation_x);
	char_widget->character = character_manager.get_current_character();

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
	for (int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(time_delta);
	}
}

void DungeonTechDemo::take_input(boundinput input, bool type)
{
	GridCharacter *current_char = character_manager.get_current_character();

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
	GridCharacter *current_char = character_manager.get_current_character();

	t_vertex camera_pos;

	if (current_char->state == GRID_MOVING)
		camera_pos = current_char->draw_position;
	else
		camera_pos = current_char->position;

	// this stuff is for the current draw order
	camera_position = t_vertex((camera_pos.x) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y));
	camera_lookat = t_vertex(camera_pos.x, 0, (camera_pos.z));

	gluLookAt((camera_pos.x * 5) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z * 5) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_pos.x * 5, 0, (camera_pos.z * 5), 0.0f, 1.0f, 0.0f);
	
	//Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
	grid_manager.draw_3d();
	//Paintbrush::stop_shader();

	// sort and draw entities
	std::vector<Entity*> sort_list;
	sort_list = entities;
	std::sort(sort_list.begin(), sort_list.end(), by_depth_entity());

	int i;
	for (i = 0; i < sort_list.size(); i++)
	{
		glPushMatrix();
		if (grid_manager.tile_map[sort_list.at(i)->position.x][sort_list.at(i)->position.z].discovered)
		{
			//Paintbrush::use_shader(Paintbrush::get_shader("point_light"));
			sort_list.at(i)->draw();
			//Paintbrush::stop_shader();
		}
		glPopMatrix();
	}
}

void DungeonTechDemo::reset()
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

void DungeonTechDemo::draw_hud()
{
	BaseTechDemo::draw();
}
