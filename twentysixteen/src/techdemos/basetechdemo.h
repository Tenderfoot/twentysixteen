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
	ListWidget *prototyping;

	void init()
	{
		TechDemoUI.add_widget(new TextWidget("Base Tech Demo", 0.5, 0.2, 0.5, 0.3));
		
		initial_list = new ListWidget({ "DATA IMPORT", "VFX", "PROTOTYPING", "QUIT" });
		initial_list->set_data(0.5, 0.4, 0.2, 0.1, true);
		TechDemoUI.add_widget(initial_list);

		filetypes_list = new ListWidget({ "Spine", "TTF/SOIL", "SDL_MIXER", "ASSIMP", "BACK" });
		filetypes_list->set_data(0.5, 0.4, 0.2, 0.1, false);
		TechDemoUI.add_widget(filetypes_list);

		vfx_list = new ListWidget({ "SHADERS", "EMITTERS", "GRASS", "LIGHTS", "BACK" });
		vfx_list->set_data(0.5, 0.4, 0.2, 0.1, false);
		TechDemoUI.add_widget(vfx_list);

		prototyping = new ListWidget({"COLLISION", "PHYSICS", "SCENE", "BACK" });
		prototyping->set_data(0.5, 0.4, 0.2, 0.1, false);
		TechDemoUI.add_widget(prototyping);

		current_list = initial_list;

		TechDemoUI.add_widget(new TextWidget("Use directions (WASD) and A (space) to select", 0.5, 0.95, 0.5, 0.05));
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

			if (input == ACTION)
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
				if (strcmp(choice, "QUIT") == 0)
				{
					exit_level = QUIT;
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
				if (strcmp(choice, "SCENE") == 0)
				{
					exit_level = TECHDEMO_SCENE;
				}
				if (strcmp(choice, "EMITTERS") == 0)
				{
					exit_level = TECHDEMO_EMITTER;
				}
				if (strcmp(choice, "PHYSICS") == 0)
				{
					exit_level = TECHDEMO_PHYSICS;
				}
				if (strcmp(choice, "LIGHTS") == 0)
				{
					exit_level = TECHDEMO_LIGHTS;
				}
				if (strcmp(choice, "DATA IMPORT") == 0)
				{
					switch_to(filetypes_list);
				}
				if (strcmp(choice, "VFX") == 0)
				{
					switch_to(vfx_list);
				}
				if (strcmp(choice, "PROTOTYPING") == 0)
				{
					switch_to(prototyping);
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
	}

	void draw() 
	{
		TechDemoUI.draw();
	}
	
};
