#pragma once

/* Class BaseTechDemo */
// The BaseTechDemo class will be inherited by each
// of the tech demos - demonstrations of the core
// functionality I'll be building into this basecode.
// This class will handle drawing the demo name at the top
// and going back to the main menu when escpae is pressed.

#include "../paintbrush.h"
#include "../level.h"
#include "../base_user_interface.h"
#include "../audio_controller.h"

class BaseTechDemo : public Level
{
public:

	BaseTechDemo()
	{
	}

	char *techdemo_title;
	BaseUserInterface TechDemoUI;
	ListWidget *current_list;
	ListWidget *initial_list;
	ListWidget *filetypes_list;
	ListWidget *vfx_list;
	ListWidget *physics_list;
	ListWidget *scene_list;
	ListWidget *prototyping;

	void init()
	{
		TechDemoUI.add_widget(new TextWidget("Base Tech Demo", 0.5, 0.175, 0.5, 0.3));
		
		initial_list = new ListWidget({ "DATA IMPORT", "VFX", "PLATFORMER", "GRID STUFF", "OTHER", "QUIT" });
		initial_list->set_data(0.5, 0.35, 0.2, 0.1, true);
		TechDemoUI.add_widget(initial_list);

		filetypes_list = new ListWidget({ "Spine", "TTF/SOIL", "SDL_MIXER", "ASSIMP", "COLLISION", "BACK" });
		filetypes_list->set_data(0.5, 0.35, 0.2, 0.1, false);
		TechDemoUI.add_widget(filetypes_list);

		vfx_list = new ListWidget({ "SHADERS", "EMITTERS", "GRASS", "LIGHTS", "BACK" });
		vfx_list->set_data(0.5, 0.35, 0.2, 0.1, false);
		TechDemoUI.add_widget(vfx_list);

		physics_list = new ListWidget({ "MEADOW", "PUZZLE", "BACK" });
		physics_list->set_data(0.5, 0.4, 0.2, 0.1, false);
		TechDemoUI.add_widget(physics_list);

		prototyping = new ListWidget({ "BREAKOUT",  "BACK" });
		prototyping->set_data(0.5, 0.4, 0.2, 0.1, false);
		TechDemoUI.add_widget(prototyping);

		/* no longer used */
		scene_list = new ListWidget({ "FOG_OF_WAR", "DUNGEON", "BACK" });
		scene_list->set_data(0.5, 0.35, 0.2, 0.1, false);
		TechDemoUI.add_widget(scene_list);

		current_list = initial_list;

		TechDemoUI.add_widget(new TextWidget("Use directions (WASD) and A (space) to select - OR THE MOUSE!", 0.5, 0.975, 0.65, 0.05));
	}

	void reset()
	{

	}

	void switch_to(ListWidget *to_switch)
	{
		current_list->visible = false;
		current_list = to_switch;
		current_list->visible = true;
	}

	void take_input(boundinput input, bool type)
	{
		if (type == true)
		{
			if (input == UP)
			{
				current_list->previous_item();
			}

			if (input == DOWN)
			{
				current_list->next_item();
			}

			if (input == ACTION || input == LMOUSE)
			{
				char *choice = current_list->list_items[current_list->current_selection];
				if (strcmp(choice, "Spine") == 0)
				{
					exit_level = TECHDEMO_SPINE;
				}
				if (strcmp(choice, "TTF/SOIL") == 0)
				{
					exit_level = TECHDEMO_TTF;
				}
				if (strcmp(choice, "SDL_MIXER") == 0)
				{
					exit_level = TECHDEMO_AUDIO;
				}
				if (strcmp(choice, "ASSIMP") == 0)
				{
					exit_level = TECHDEMO_MODEL;
				}
				if (strcmp(choice, "SHADERS") == 0)
				{
					exit_level = TECHDEMO_SHADER;
				}
				if (strcmp(choice, "MEADOW") == 0)
				{
					exit_level = TECHDEMO_MEADOW;
				}
				if (strcmp(choice, "QUIT") == 0)
				{
					exit_level = QUIT;
				}
				if (strcmp(choice, "COLLISION") == 0)
				{
					exit_level = TECHDEMO_MODELTOPOLY;
				}
				if (strcmp(choice, "GRASS") == 0)
				{
					exit_level = TECHDEMO_GRASS;
				}
				if (strcmp(choice, "BASE SCENE") == 0)
				{
					exit_level = TECHDEMO_SCENE;
				}
				if (strcmp(choice, "EMITTERS") == 0)
				{
					exit_level = TECHDEMO_EMITTER;
				}
				if (strcmp(choice, "BREAKOUT") == 0)
				{
					exit_level = TECHDEMO_BREAKOUT;
				}
				if (strcmp(choice, "FOG_OF_WAR") == 0)
				{
					exit_level = TECHDEMO_FOGOFWAR;
				}
				if (strcmp(choice, "PHYS") == 0)
				{
					exit_level = TECHDEMO_PHYSICS;
				}
				if (strcmp(choice, "LIGHTS") == 0)
				{
					exit_level = TECHDEMO_LIGHTS;
				}
				if (strcmp(choice, "POST") == 0)
				{
					exit_level = TECHDEMO_POSTPROCESS;
				}
				if (strcmp(choice, "ENEMY") == 0)
				{
					exit_level = TECHDEMO_ENEMY;
				}
				if (strcmp(choice, "GRAVEYARD") == 0)
				{
					exit_level = TECHDEMO_GRAVEYARD;
				}
				if (strcmp(choice, "MEADOW") == 0)
				{
					exit_level = TECHDEMO_MEADOW;
				}
				if (strcmp(choice, "PUZZLE") == 0)
				{
					exit_level = TECHDEMO_MARSH;
				}
				if (strcmp(choice, "TOWN") == 0)
				{
					exit_level = TECHDEMO_TOWN;
				}
				if (strcmp(choice, "DUNGEON") == 0)
				{
					exit_level = TECHDEMO_DUNGEON;
				}
				if (strcmp(choice, "DATA IMPORT") == 0)
				{
					switch_to(filetypes_list);
				}
				if (strcmp(choice, "VFX") == 0)
				{
					switch_to(vfx_list);
				}
				if (strcmp(choice, "PLATFORMER") == 0)
				{
					switch_to(physics_list);
				}
				if (strcmp(choice, "OTHER") == 0)
				{
					switch_to(prototyping);
				}
				if (strcmp(choice, "GRID STUFF") == 0)
				{
					switch_to(scene_list);
				}
				if (strcmp(choice, "BACK") == 0)
				{
					switch_to(initial_list);
				}
			}

			if (input == BACK)
				switch_to(initial_list);
		}
	}

	void run(float time_delta)
	{
		if (mousex > current_list->x*res_width-(0.5*current_list->width*res_width) && mousex < current_list->x*res_width + (0.5*current_list->width*res_width) &&
			mousey > current_list->y*res_height - (0.5*current_list->height*res_height) && mousey < current_list->y*res_height - (0.5*current_list->height*res_height) + (current_list->height*res_height)*current_list->list_items.size())
		{
			float delta = (current_list->height*res_height);
			int index = int((mousey - (current_list->y*res_height - (0.5*current_list->height*res_height))) / delta);
			
			current_list->current_selection = index;
		}
	}

	void draw() 
	{
		TechDemoUI.draw();
	}
	
};
