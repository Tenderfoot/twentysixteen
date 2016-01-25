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
	ListWidget *my_list;

	void init()
	{
		TechDemoUI.add_widget(new TextWidget("Base Tech Demo", 0.5, 0.2, 0.5, 0.3));
		my_list = new ListWidget({ "Spine", "TTF/SOIL", "SDL_MIXER", "ASSIMP" });
		TechDemoUI.add_widget(my_list);
		TechDemoUI.add_widget(new TextWidget("Use directions (WASD) and A (space) to select", 0.5, 0.9, 0.5, 0.05));
	}

	void take_input(boundinput input, bool type)
	{
		if (type == true)
		{
			if (input == UP)
			{
				my_list->previous_item();
			}

			if (input == DOWN)
			{
				my_list->next_item();
			}

			if (input == ACTION)
			{
				char *choice = my_list->list_items[my_list->current_selection];
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
			}

			if (input == BACK)
				exit_level = QUIT;
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
