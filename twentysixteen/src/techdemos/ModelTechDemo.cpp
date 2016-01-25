
#include "modeltechdemo.h"

void ModelTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Model Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	mymodel = ModelData::import("just_tree.3DS", 0.002);

}

void ModelTechDemo::run()
{
}

void ModelTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void ModelTechDemo::draw()
{
	BaseTechDemo::draw();
	Paintbrush::draw_model(mymodel);
}
