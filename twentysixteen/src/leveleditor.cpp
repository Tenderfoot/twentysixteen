
#include "leveleditor.h"
#include <iostream>
#include <sstream>
#include <fstream>

void LevelEditor::take_input(boundinput input, bool type)
{
	if (input == NEXT && type == true)
	{
		current_entity += 1;
		current_entity = current_entity % entities->size();
		while (entities->at(current_entity)->type == 1)
		{
			current_entity += 1;
			current_entity = current_entity % entities->size();
		}
	}
	if (input == PREVIOUS && type == true)
	{
		current_entity -= 1;
		current_entity = current_entity % entities->size();
		while (entities->at(current_entity)->type == 1)
		{
			current_entity += 1;
			current_entity = current_entity % entities->size();
		}
	}

	if (input == RIGHT && type == true)
	{
		entities->at(current_entity)->position.x += 1;
	}

	if (input == LEFT && type == true)
	{
		entities->at(current_entity)->position.x -= 1;
	}

	if (input == UP && type == true)
	{
		entities->at(current_entity)->position.y += 1;
	}

	if (input == DOWN && type == true)
	{
		entities->at(current_entity)->position.y -= 1;
	}

	if (input == EDITOR_SAVE && type == true)
	{
		write_level();
	}
}

void LevelEditor::update()
{
	camera_position.x = entities->at(current_entity)->position.x;
	camera_position.y = entities->at(current_entity)->position.y;
}

void LevelEditor::read_level()
{
	std::ifstream in("data/levels/test.txt");
	std::string line;

	Entity *new_entity;

	// get number of entities
	std::getline(in, line);
	int number = std::stoi(line);
	printf("number of entities: %d\n", number);
	
	while (std::getline(in, line))
	{
		// for each entity
		if (line == "PlayerEntity")
		{
			new_entity = new PlayerEntity();
			// position information
			std::getline(in, line, ',');
			new_entity->position.x = std::stoi(line);
			std::getline(in, line, ',');
			new_entity->position.y = std::stoi(line);
			std::getline(in, line, ',');
			new_entity->position.z = std::stoi(line);
			printf("Position: %f, %f, %f\n", new_entity->position.x, new_entity->position.y, new_entity->position.z);
			
			// size information
			std::getline(in, line, ',');
			new_entity->size.x = std::stoi(line);
			std::getline(in, line, ',');
			new_entity->size.y = std::stoi(line);
			std::getline(in, line, ',');
			new_entity->size.z = std::stoi(line);
			printf("Size: %f, %f, %f\n", new_entity->size.x, new_entity->size.y, new_entity->size.z);

			((PlayerEntity*)new_entity)->init();
			entities->push_back(new_entity);
		}
	}
}

void LevelEditor::write_level() 
{
	std::ofstream myfile;
	myfile.open("data/levels/test.txt");
	myfile << "1\n";

	int i;
	for (i = 0; i < entities->size(); i++)
	{
		if (entities->at(i)->type == PLAYER_ENTITY)
		{
			myfile << "PlayerEntity\n";
			myfile << entities->at(i)->position.x << "," << entities->at(i)->position.y << "," << entities->at(i)->position.z << ",";
			myfile << "1,3,1\n";
		}
	}

	myfile.close();
}