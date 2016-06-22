
#include "leveleditor.h"

void LevelEditor::take_input(boundinput input, bool type)
{
	if (input == RIGHT && type == true)
	{
		current_entity+=1;
		current_entity = current_entity % entities->size();
		while (entities->at(current_entity)->type == 1)
		{
			current_entity += 1;
			current_entity = current_entity % entities->size();
		}
		printf("type: %d\n", entities->at(current_entity)->type);
	}
}

void LevelEditor::update()
{
	camera_position.x = entities->at(current_entity)->position.x;
	camera_position.y = entities->at(current_entity)->position.y;
}