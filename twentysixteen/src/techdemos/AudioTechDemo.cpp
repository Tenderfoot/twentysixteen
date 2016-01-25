
#include "audiotechdemo.h"

void AudioTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Audio Tech Demo", 0.5, 0.2, 0.5, 0.3));

	TechDemoUI.add_widget(new TextWidget("press W for tom1", 0.5, 0.4, 0.2, 0.05));
	TechDemoUI.add_widget(new TextWidget("press S for tom2", 0.5, 0.5, 0.2, 0.05));
	TechDemoUI.add_widget(new TextWidget("press A/D for tom3", 0.5, 0.6, 0.2, 0.05));

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
			AudioController::play_sound("tom1");

		if (input == DOWN)
			AudioController::play_sound("tom2");

		if (input == ACTION)
			AudioController::play_sound("tom3");
	}
}

void AudioTechDemo::draw()
{
	BaseTechDemo::draw();
}
