#include "fogofwartechdemo.h"
#include "../game_entity.h"


// TODO:

// RTS Stuff
	// Building Buildings
		// when gatherer reaches destination, if is in build mode, make the building
			// mostly create the entity and spawn it
	// Units Attacking

// TILE STUFF:
	// add dark grass
	// split vision blockage between water and rocks
	// paint terrain (edit mode for grid stuff??)


/************ BUG AND TODO PILE ***************/
/******************* TODOS ********************/
	// TODO: Choose spot around building if spot is takem
/******************  BUGS *********************/
	// BUG: Draw order should use draw position not position
	// Bug: Crashes when greenbox leaves area
/**********************************************/

void FogOfWarTechDemo::init()
{
	TechDemoUI.add_widget(new UIImage(0.5, 0.9, 1.01, 0.2, Paintbrush::Soil_Load_Texture("data/images/HUD.png", false, false)));
	TechDemoUI.add_widget(new MapWidget(&grid_manager));
	char_widget = new CharacterWidget(nullptr);
	TechDemoUI.add_widget(char_widget);

	green_box = new GreenBox();
	TechDemoUI.add_widget(green_box);

	grid_manager.entities = &entities;
	grid_manager.init();

	// make the player
	new_player = new FOWPlayer();
	new_player->grid_manager = &grid_manager;
	new_player->entities = &entities;

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
			new_character->position = current_entity->position;
			entities.push_back(new_character);
		}
	}

	FOWBuilding *new_building = new FOWTownHall(9,7, 3);
	entities.push_back(new_building);
	new_building = new FOWGoldMine(22, 7, 3);
	entities.push_back(new_building);

	lookmode = false;

	// this is actually important, need to fix lookat calc
	camera_rotation_y = 1;
	camera_distance = 50.0f;
	camera_pos.x = 15;
	camera_pos.z = 10;


	reset();
}

void FogOfWarTechDemo::run(float time_delta)
{
	// green box (this updates info for the widget that draws it)
	green_box->width = mousex;
	green_box->height = mousey;

	// this gives grid manager the mouse x y, which is used by player to draw buildings
	grid_manager.convert_mouse_coords(mouse_in_space);

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

	if (input == LMOUSE && type == true)
	{
		green_box->x = mousex;
		green_box->y = mousey;
		green_box->mouse_in_space = mouse_in_space;
		green_box->visible = true;
	}

	if (input == LMOUSE && type == false)
	{

		if (new_player->selection_group.selected_characters.size() == 1)
		{
			if (new_player->selection_group.selected_characters.at(0)->type == FOW_GATHERER)
			{
				FOWGatherer *gatherer = ((FOWGatherer*)new_player->selection_group.selected_characters.at(0));
				if (gatherer->build_mode && gatherer->good_spot)
				{
					gatherer->give_command(FOWCommand(BUILD_BUILDING, t_vertex(grid_manager.mouse_x, 0.0f, grid_manager.mouse_y)));
				}
			}
		}

		green_box->visible = false;
		new_player->get_selection(grid_manager.convert_mouse_coords(green_box->mouse_in_space), grid_manager.convert_mouse_coords(mouse_in_space));
		if (new_player->selection_group.selected_characters.size() > 0)
			char_widget->character = new_player->selection_group.selected_characters.at(0);
		else
			char_widget->character = nullptr;

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

	if (input == HOTKEY_B && type == true)
	{
		if (new_player->selection_group.selected_characters.size() == 1)
		{
			if (new_player->selection_group.selected_characters.at(0)->type == FOW_GATHERER)
			{
				((FOWGatherer*)new_player->selection_group.selected_characters.at(0))->build_mode = true;
			}
		}
	}

	if (input == USE)
	{
		new_player->queue_add_toggle = type;

		if (new_player->selection_group.selected_characters.size() == 1)
		{
			if (new_player->selection_group.selected_characters.at(0)->type == FOW_TOWNHALL)
			{
				if (new_player->gold > 0)
				{
					new_player->gold--;
					new_player->selection_group.selected_characters.at(0)->process_command(FOWCommand(BUILD_UNIT, FOW_GATHERER));
					new_gatherer = new FOWGatherer();
					new_gatherer->spine_data.load_spine_data("everybody");
					spSkeleton_setSkinByName(new_gatherer->spine_data.skeleton, "farm");
					new_gatherer->spine_data.animation_name = "idle";
					new_gatherer->spine_data.looping = true;
					new_gatherer->grid_manager = &grid_manager;
					new_gatherer->position = t_vertex(new_player->selection_group.selected_characters.at(0)->position.x + 4, 0.0f, new_player->selection_group.selected_characters.at(0)->position.z);
					entities.push_back(new_gatherer);
				}
				else
					printf("not enough minerals\n");
			}
		}
	}

	if (input == RMOUSE && type == true)
	{
		t_vertex hit_position = grid_manager.convert_mouse_coords(mouse_in_space);
		Entity *hit_target = nullptr;

		// lets see if theres something on the hit position...
		for (int i = 0; i < entities.size(); i++)
		{
			Entity *test = entities.at(i);
			if (new_player->is_selectable(test->type))
			{
				if (test->position.x == hit_position.x && test->position.z == hit_position.z
					&& test->position.x == hit_position.x && test->position.z == hit_position.z)
				{
					hit_target = test;
				}
			}
		}

		if (new_player->selection_group.selected_characters.size() > 0)
		{
			for (int i = 0; i < new_player->selection_group.selected_characters.size(); i++)
			{	
				if (new_player->selection_group.selected_characters.at(i)->type == FOW_CHARACTER)
				{
					if (new_player->queue_add_toggle == false)
						new_player->selection_group.selected_characters.at(i)->command_queue.clear();
					((FOWCharacter*)new_player->selection_group.selected_characters.at(i))->give_command(FOWCommand(MOVE, t_vertex(hit_position.x + i, 0.0f, hit_position.z + i % 2)));
				}

				if (new_player->selection_group.selected_characters.at(i)->type == FOW_GATHERER)
				{
					FOWGatherer *gatherer = (FOWGatherer*)new_player->selection_group.selected_characters.at(i);

					if (hit_target != nullptr)
					{
						if (hit_target->type == FOW_GOLDMINE)
						{
							gatherer->give_command(FOWCommand(GATHER, hit_target));
						}
					}
					else
					{
						if (new_player->queue_add_toggle == false)
							new_player->selection_group.selected_characters.at(i)->command_queue.clear();
						((FOWCharacter*)new_player->selection_group.selected_characters.at(i))->give_command(FOWCommand(MOVE, t_vertex(hit_position.x + i, 0.0f, hit_position.z + i % 2)));
					}
				}
			}
		}
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
}

void FogOfWarTechDemo::draw()
{
	// this stuff is to sort the draw order - used in level
	camera_position = t_vertex((camera_pos.x) + ((sin(camera_rotation_x)*camera_distance))*sin(camera_rotation_y), camera_distance*cos(camera_rotation_y), (camera_pos.z) + ((cos(camera_rotation_x)*camera_distance))*sin(camera_rotation_y));
	camera_lookat = t_vertex(camera_pos.x, 0, (camera_pos.z));

	gluLookAt((camera_pos.x * 5), camera_distance, (camera_pos.z * 5), camera_pos.x * 5, 0, (camera_pos.z * 5), 0.0f, 0.0f, -1.0f);

	// draw the map
	grid_manager.draw_autotile();

	// draw boxes around selected characters
	draw_selections();

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

void FogOfWarTechDemo::draw_selections()
{
	int i;
	t_vertex draw_position;
	int draw_size;
	// This is for the selection border
	// needs to be drawn before characters - or does it
	for (i = 0; i < entities.size(); i++)
	{
		glPushMatrix();
		if (new_player->is_selectable(entities.at(i)->type))
		{
			if (entities.at(i)->type == FOW_CHARACTER || entities.at(i)->type == FOW_GATHERER)
			{
				FOWCharacter *fow_character = (FOWCharacter*)entities.at(i);
				draw_position = fow_character->draw_position;
			}
			if (entities.at(i)->type == FOW_BUILDING || entities.at(i)->type == FOW_TOWNHALL || entities.at(i)->type == FOW_GOLDMINE)
			{
				draw_position = entities.at(i)->position;
			}

			if (((FOWSelectable*)entities.at(i))->selected)
			{
				glColor3f(0.5f, 1.0f, 0.5f);
				glDisable(GL_TEXTURE_2D);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
					glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) - 2.5);
					glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) + 2.5);
					glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) - 2.5);
					glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) - 2.5);
					glVertex3f((draw_position.x * 5) - 2.5, 0.1f, (draw_position.z * 5) + 2.5);
					glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) + 2.5);
					glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) - 2.5);
					glVertex3f((draw_position.x * 5) + 2.5, 0.1f, (draw_position.z * 5) + 2.5);
				glEnd();
				glColor3f(1.0f, 1.0f, 1.0f);
				glEnable(GL_TEXTURE_2D);
			}
		}
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
