#include "fogofwartechdemo.h"
#include "../game_entity.h"
#include "../audio_controller.h"

// TODO:

// RTS Stuff

// TILE STUFF:
	// add dark grass
	// split vision blockage between water and rocks

/************ BUG AND TODO PILE ***************/
/******************* TODOS ********************/
	// TODO: Choose spot around building if spot is takem
/******************  BUGS *********************/
	// BUG: Draw order should use draw position not position
/**********************************************/

void FogOfWarTechDemo::init()
{

	// music?
	//AudioController::play_sound("data/sounds/battle.wav");

	// UI
	TechDemoUI.add_widget(new UIImage(0.5, 0.9, 1.01, 0.2, Paintbrush::Soil_Load_Texture("data/images/HUD.png", false, false)));
	TechDemoUI.add_widget(new MapWidget(&grid_manager));
	char_widget = new CharacterWidget(nullptr);
	TechDemoUI.add_widget(char_widget);

	// gridmanager
	grid_manager.entities = &entities;
	grid_manager.init();

	// make the player
	new_player = &player;
	new_player->grid_manager = &grid_manager;
	new_player->entities = &entities;
	TechDemoUI.add_widget(new_player->green_box);
	new_player->mousex = &mousex;
	new_player->mousey = &mousey;

	// make the player
	new_player = &editor;
	new_player->grid_manager = &grid_manager;
	new_player->entities = &entities;
	TechDemoUI.add_widget(new_player->green_box);
	new_player->mousex = &mousex;
	new_player->mousey = &mousey;

	new_player = &player;

	// this should go somewhere
	for (int i = 0; i < entities.size(); i++)
	{
		Entity *current_entity = entities.at(i);
		if (current_entity->type == GRID_SPAWNPOINT)
		{
			new_character = new FOWGatherer();
			new_character->spine_data.load_spine_data("everybody");
			spSkeleton_setSkinByName(new_character->spine_data.skeleton, "farm");
			new_character->spine_data.animation_name = "idle";
			new_character->spine_data.looping = true;
			new_character->grid_manager = &grid_manager;
			new_character->owner = new_player;
			new_character->position = current_entity->position;
			entities.push_back(new_character);
		}
	}

	// this should be part of the map
	FOWBuilding *new_building = new FOWTownHall(9,7, 3);
	entities.push_back(new_building);
	new_building = new FOWGoldMine(22, 7, 3);
	entities.push_back(new_building);

	lookmode = false;

	// this is actually important, need to fix lookat calc
	camera_rotation_y = 1;
	reset();
}

void FogOfWarTechDemo::run(float time_delta)
{
	// this gives grid manager the mouse x y, which is used by player to draw buildings
	grid_manager.convert_mouse_coords(mouse_in_space);

	// update player
	new_player->mouse_in_space = mouse_in_space;
	new_player->update();

	// Loop through and update entities. This should stay and other things
	// should be refactored out.
	for (int i = 0; i < entities.size(); i++)
	{
		entities.at(i)->update(time_delta);
	}
}

void FogOfWarTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;

	if (input == EDITOR_CREATE_MODE && type == true)
	{
		new_player = &editor;
	}
	
	if (input == EDITOR_PLAY_MODE && type == true)
	{
		new_player = &player;
	}

	new_player->take_input(input, type);
}

void FogOfWarTechDemo::draw()
{
	// this stuff is to sort the draw order - used in level
	camera_position = new_player->camera_pos;
	camera_lookat = t_vertex(camera_pos.x, 0, (camera_pos.z));
	float camera_distance = new_player->camera_distance;

	gluLookAt((camera_position.x * 5), camera_distance, (camera_position.z * 5), camera_position.x * 5, 0, (camera_position.z * 5), 0.0f, 0.0f, -1.0f);

	// draw the map
	grid_manager.draw_autotile();

	// draw things specific to the player i.e. planned buildings, unit paths maybe
	new_player->draw();

	// sort and draw entities
	std::vector<Entity*> sort_list;
	sort_list = entities;
	std::sort(sort_list.begin(), sort_list.end(), by_depth_entity());

	for (int i = 0; i < sort_list.size(); i++)
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
