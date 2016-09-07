
#include "JokeTechDemo.h"

void JokeTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Breakout", 0.5, 0.1, 0.5, 0.15));
	TechDemoUI.add_widget(new TextWidget("indirectly produced by Sam Neale", 0.5, 0.2, 0.3, 0.05));
	TechDemoUI.add_widget(new TextWidget("\"My name is Willy\"", 0.5, 0.775, 0.3, 0.05));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go start", 0.5, 0.9, 0.5, 0.05));
	spineboy.load_spine_data("willy");
	spineboy.animation_name = "mouth_open";
	spineboy.looping = true;
}

void JokeTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);
}

void JokeTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BREAKOUT;
}

void JokeTechDemo::draw()
{
	BaseTechDemo::draw();

	glPushMatrix();
	glTranslatef(0.0f, -1.0f, -10.0f);
	glScalef(0.01f, 0.01f, 0.01f);

	spineboy.draw();

	glPopMatrix();
}
