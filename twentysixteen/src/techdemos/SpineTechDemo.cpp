
#include "spinetechdemo.h"

void SpineTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Spine Tech Demo", 0.5, 0.2, 0.5, 0.3));
	spineboy.load_spine_data("spineboy");
}

void SpineTechDemo::run()
{
	spineboy.update_skeleton();
}

void SpineTechDemo::take_input(boundinput input, bool type)
{
}

void SpineTechDemo::draw()
{
	BaseTechDemo::draw();
	spineboy.draw();
}
