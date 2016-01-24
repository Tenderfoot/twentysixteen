
#include "ttftechdemo.h"

void TTFTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("TTF Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	TechDemoUI.add_widget(new UIImage(Paintbrush::get_texture("data/images/controller_splash.png", false)));
}

void TTFTechDemo::run()
{
}

void TTFTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void TTFTechDemo::draw()
{
	BaseTechDemo::draw();
}
