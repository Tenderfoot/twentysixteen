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

class BaseTechDemo : public Level
{
public:

	BaseTechDemo()
	{
	}

	char *techdemo_title;
	BaseUserInterface TechDemoUI;

	void init()
	{
		TechDemoUI.add_widget(new TextWidget("Base Tech Demo", 0.5, 0.2, 0.5, 0.3));
		TechDemoUI.add_widget(new ListWidget({"Spine", "TTF/SOIL", "SDL_MIXER", "ASSIMP"}));
		TechDemoUI.add_widget(new TextWidget("Use directions (WASD) and A (space) to select", 0.5, 0.9, 0.5, 0.05));
	}

	void run()
	{
	}

	void draw() 
	{
		TechDemoUI.draw();
	}
	
};
