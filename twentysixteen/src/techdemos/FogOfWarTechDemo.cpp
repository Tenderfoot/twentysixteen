#include "fogofwartechdemo.h"
#include "../game_entity.h"


// TODO:

// RTS Stuff
	// Make a building (Town Hall?)
	// Make a unit (gatherer)
	// produce unit out of building (HUD buttons?)
	// BUG: Draw order should use draw position not position
	// Bug: Crashes when greenbox leaves area

// TILE STUFF:
	// add dark grass
	// add trees
		// fix
	// split vision blockage between water and rocks
	// paint terrain (edit mode for grid stuff??)


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
			new_character = new FOWCharacter();
			new_character->spine_data.load_spine_data("everybody");
			spSkeleton_setSkinByName(new_character->spine_data.skeleton, "knight");
			new_character->spine_data.animation_name = "idle";
			new_character->spine_data.looping = true;
			new_character->grid_manager = &grid_manager;
			new_character->position = current_entity->position;
			entities.push_back(new_character);
		}
	}

	FOWBuilding *new_building = new FOWBuilding(2,2,3);
	entities.push_back(new_building);


	lookmode = false;

	// this is actually important, need to fix lookat calc
	camera_rotation_y = 1;
	camera_distance = 25.0f;

	reset();
}

void FogOfWarTechDemo::run(float time_delta)
{
	// green box (this updates info for the widget that draws it)
	green_box->width = mousex;
	green_box->height = mousey;

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
		green_box->visible = false;
		new_player->get_selection(grid_manager.convert_mouse_coords(green_box->mouse_in_space), grid_manager.convert_mouse_coords(mouse_in_space));
		if(new_player->selection_group.selected_characters.size()>0)
			char_widget->character = new_player->selection_group.selected_characters.at(0);
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

	if (input == USE)
	{
		new_player->queue_add_toggle = type;
	}

	if (input == RMOUSE && type == true)
	{
		t_vertex hit_position = grid_manager.convert_mouse_coords(mouse_in_space);

		if (new_player->selection_group.selected_characters.size() > 0)
		{
			for (int i = 0; i < new_player->selection_group.selected_characters.size(); i++)
			{
				if(new_player->queue_add_toggle == false)
					new_player->selection_group.selected_characters.at(i)->command_queue.clear();
				new_player->selection_group.selected_characters.at(i)->give_command(FOWCommand(MOVE, t_vertex(hit_position.x+i, 0.0f, hit_position.z+i%2)));
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
	// This is for the selection border
	// needs to be drawn before characters - or does it
	for (i = 0; i < entities.size(); i++)
	{
		glPushMatrix();
		if (entities.at(i)->type == FOW_CHARACTER)
		{
			FOWCharacter *fow_character = (FOWCharacter*)entities.at(i);

			if (fow_character->selected)
			{
				glColor3f(0.5f, 1.0f, 0.5f);
				glDisable(GL_TEXTURE_2D);
				glLineWidth(1.0f);
				glBegin(GL_LINES);
					glVertex3f((fow_character->draw_position.x * 5) - 2.5, 0.1f, (fow_character->draw_position.z * 5) - 2.5);
					glVertex3f((fow_character->draw_position.x * 5) - 2.5, 0.1f, (fow_character->draw_position.z * 5) + 2.5);
					glVertex3f((fow_character->draw_position.x * 5) - 2.5, 0.1f, (fow_character->draw_position.z * 5) - 2.5);
					glVertex3f((fow_character->draw_position.x * 5) + 2.5, 0.1f, (fow_character->draw_position.z * 5) - 2.5);
					glVertex3f((fow_character->draw_position.x * 5) - 2.5, 0.1f, (fow_character->draw_position.z * 5) + 2.5);
					glVertex3f((fow_character->draw_position.x * 5) + 2.5, 0.1f, (fow_character->draw_position.z * 5) + 2.5);
					glVertex3f((fow_character->draw_position.x * 5) + 2.5, 0.1f, (fow_character->draw_position.z * 5) - 2.5);
					glVertex3f((fow_character->draw_position.x * 5) + 2.5, 0.1f, (fow_character->draw_position.z * 5) + 2.5);
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
