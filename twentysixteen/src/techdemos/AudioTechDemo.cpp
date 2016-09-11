
#include "audiotechdemo.h"

void AudioTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Audio Tech Demo", 0.5, 0.2, 0.5, 0.3));

	TechDemoUI.add_widget(new TextWidget("press W for tom1", 0.5, 0.4, 0.2, 0.05));
	TechDemoUI.add_widget(new TextWidget("press S for tom2", 0.5, 0.5, 0.2, 0.05));
	TechDemoUI.add_widget(new TextWidget("press SPACE for tom3", 0.5, 0.6, 0.2, 0.05));

	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
}

void AudioTechDemo::run(float time_delta)
{
}

void AudioTechDemo::take_input(boundinput input, bool type)
{
	if (type == true)
	{
		if (input == BACK)
			exit_level = TECHDEMO_BASE;

		if (input == UP)
			AudioController::play_sound("data/sounds/tom1.wav");

		if (input == DOWN)
			AudioController::play_sound("data/sounds/tom2.wav");

		if (input == ACTION)
			AudioController::play_sound("data/sounds/tom3.wav");
	}
}

void AudioTechDemo::draw()
{
	BaseTechDemo::draw();
}
