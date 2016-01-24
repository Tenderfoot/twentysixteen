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
		this->techdemo_title = "Base Tech Demo";
	}

	char *techdemo_title;
	BaseUserInterface TechDemoUI;

	void init()
	{
		TechDemoUI.add_widget(new TextWidget(this->techdemo_title, 0.5, 0.2, 0.5, 0.3));
	}

	void draw() 
	{
		TechDemoUI.draw();
	}
	
};
