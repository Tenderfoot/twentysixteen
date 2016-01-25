
#include "modeltechdemo.h"

void ModelTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Model Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	mymodel = ModelData::import("just_tree.3DS", 0.002);

}

void ModelTechDemo::run(float time_delta)
{
	rotation += (time_delta/10);
}

void ModelTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void ModelTechDemo::draw()
{
	BaseTechDemo::draw();
	
	glPushMatrix();
		glTranslatef(0.0f, -20.0f, -50.0f);
		glRotatef(rotation, 0.0f, 1.0f, 0.0f);
		Paintbrush::draw_model(mymodel);
	glPopMatrix();
}
