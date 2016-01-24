
#include "ttftechdemo.h"

void TTFTechDemo::init()
{
	BaseTechDemo::init();
	TechDemoUI.add_widget(new UIImage(Paintbrush::Soil_Load_Texture("data/images/controller_splash.png")));
}

void TTFTechDemo::run()
{
}

void TTFTechDemo::draw()
{
	BaseTechDemo::draw();
}
