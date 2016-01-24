
#include "spinetechdemo.h"

void SpineTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Spine Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	spineboy.load_spine_data("spineboy");
}

void SpineTechDemo::run()
{
	spineboy.update_skeleton();
}

void SpineTechDemo::take_input(boundinput input, bool type)
{
	if(input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void SpineTechDemo::draw()
{
	BaseTechDemo::draw();
	spineboy.draw();
}
