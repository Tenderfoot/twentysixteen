
#include "ttftechdemo.h"

void TTFTechDemo::init()
{
	BaseTechDemo::init();
	TechDemoUI.add_widget(new UIImage(Paintbrush::get_texture("data/images/controller_splash.png", false)));
}

void TTFTechDemo::run()
{
}

void TTFTechDemo::draw()
{
	BaseTechDemo::draw();
}
