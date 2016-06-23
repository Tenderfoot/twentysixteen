
#include "leveleditor.h"
#include "particles.h"

void LevelEditor::take_input(boundinput input, bool type)
{
	if (editor_mode == CREATE_MODE)
	{
		input_create(input, type);
	}
	else if(editor_mode == EDIT_MODE)
	{
		input_edit(input, type);
	}

	if (input == EDITOR_SAVE && type == true)
	{
		write_level();
	}

	if (input == EDITOR_CREATE_MODE && type == true)
	{
		if (editor_mode == PLAY_MODE)
		{
			reset_entities();
		}

		create_mode_entity->position = entities->at(current_entity)->position;
		editor_mode = CREATE_MODE;
	}

	if (input == EDITOR_EDIT_MODE && type == true)
	{
		if (editor_mode == PLAY_MODE)
		{
			reset_entities();
		}
		editor_mode = EDIT_MODE;
	}

	if (input == EDITOR_PLAY_MODE && type == true)
	{
		editor_mode = PLAY_MODE;
	}
}

void LevelEditor::reset_entities()
{
	int i;
	for (i = 0; i < entities->size(); i++)
	{
		entities->at(i)->reset();
	}
}

void LevelEditor::update()
{
	if (editor_mode == CREATE_MODE)
	{
		camera_position.x = create_mode_entity->position.x;
		camera_position.y = create_mode_entity->position.y;
	}
	else
	{
		// edit mode
		camera_position.x = entities->at(current_entity)->position.x;
		camera_position.y = entities->at(current_entity)->position.y;
	}
}

t_vertex LevelEditor::get_vertex_from_buffer(std::ifstream *in)
{
	t_vertex to_return;
	std::string line;

	std::getline(*in, line, ',');
	to_return.x = std::stof(line);
	std::getline(*in, line, ',');
	to_return.y = std::stof(line);
	std::getline(*in, line, ',');
	to_return.z = std::stof(line);

	return to_return;
}

void LevelEditor::read_level(std::string level_name)
{
	std::stringstream filename;
	filename << "data/levels/" << level_name.c_str() << ".txt";
	std::ifstream in(filename.str());
	std::string line;

	Entity *new_entity;

	// get number of entities
	std::getline(in, line);
	int number = std::stoi(line);
	printf("number of entities: %d\n", number);
	
	t_vertex new_pos, new_size, new_color;

	while (std::getline(in, line))
	{
		printf("%s\n", line.c_str());
		// for each entity
		if (line == "PlayerEntity")
		{
			// gather information
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			new_entity = new PlayerEntity(new_pos, new_size, t_vertex(1.0f,1.0f,1.0f));

			((PlayerEntity*)new_entity)->init();
			((PlayerEntity*)new_entity)->spine_data.setslots();
			entities->push_back(new_entity);
		}
		if (line == "GameEntity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			new_entity = new GameEntity(new_pos, new_size, t_vertex(1.0f,1.0f,1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = NULL;

			entities->push_back(new_entity);
		}
		if (line == "Entity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);
			new_color = get_vertex_from_buffer(&in);

			new_entity = new Entity(new_pos, new_size, new_color);
			new_entity->texture = NULL;

			entities->push_back(new_entity);
		}
		if (line == "EmitterEntity")
		{
			// gather information
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			new_entity = new ParticleEmitter();
			while (((ParticleEmitter*)new_entity)->particles.size() < 1000)
			{
				((ParticleEmitter*)new_entity)->particles.push_back(new Star);
			}
			((ParticleEmitter*)new_entity)->init(Paintbrush::get_texture("data/images/fire.png", false, false), new_pos, new_size);

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
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << "1,3,1,\n";
		}
		if (entities->at(i)->type == GAME_ENTITY)
		{
			myfile << "GameEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << "," << "\n";
		}
		if (entities->at(i)->type == ENTITY)
		{
			myfile << "Entity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->color.x << "," << entities->at(i)->color.y << "," << entities->at(i)->color.z << "," << "\n";
		}
		if (entities->at(i)->type == EMITTER_ENTITY)
		{
			myfile << "EmitterEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << "," << "\n";
		}
	}

	myfile.close();
}

void LevelEditor::input_edit(boundinput input, bool type)
{
	if (input == EDITOR_DELETE && type == true)
	{	

		int i;
		for (i = 0; i < render_targets->size(); i++)
		{
			if (entities->at(current_entity) == render_targets->at(i).the_entity)
			{
				printf("hit this!\n");
				render_targets->erase(render_targets->begin() + i);
			}
		}

		delete entities->at(current_entity);
		entities->erase(entities->begin() + current_entity);
		current_entity = 0;
	}

	if (input == NEXT && type == true)
	{
		current_entity += 1;
		current_entity = current_entity % entities->size();
		while (entities->at(current_entity)->type == GRASS_ENTITY)
		{
			current_entity += 1;
			current_entity = current_entity % entities->size();
		}
	}
	if (input == PREVIOUS && type == true)
	{
		current_entity -= 1;
		current_entity = current_entity % entities->size();
		while (entities->at(current_entity)->type == GRASS_ENTITY)
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

	entities->at(current_entity)->initial_position = entities->at(current_entity)->position;
}

void LevelEditor::build_entity()
{
	t_vertex pos;
	pos = create_mode_entity->position;
	delete create_mode_entity;
	switch (current_type)
	{
		case ENTITY:
			create_mode_entity = new Entity(t_vertex(pos.x, pos.y, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 0));
			break;
		case GAME_ENTITY:
			create_mode_entity = new GameEntity(t_vertex(pos.x, pos.y, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 1));
			break;
		case PLAYER_ENTITY:
			create_mode_entity = new PlayerEntity(t_vertex(pos.x, pos.y, 0), t_vertex(1, 3, 1), t_vertex(1, 0, 1));
			((PlayerEntity*)create_mode_entity)->init();
			((PlayerEntity*)create_mode_entity)->position = pos;
			((PlayerEntity*)create_mode_entity)->spine_data.setslots();
			break;
		default:
			create_mode_entity = new Entity(t_vertex(pos.x, pos.y, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 0));
			break;
	}
}

void LevelEditor::input_create(boundinput input, bool type)
{
	// these should change the entity type
	if (input == NEXT && type == true)
	{
		current_type = (current_type + 1) % 4;
		build_entity();
	}

	if (input == PREVIOUS && type == true)
	{
		current_type = (current_type - 1) % 4;
		build_entity();
	}

	// movement
	if (input == RIGHT && type == true)
	{
		create_mode_entity->position.x += 1;
	}

	if (input == LEFT && type == true)
	{
		create_mode_entity->position.x -= 1;
	}

	if (input == UP && type == true)
	{
		create_mode_entity->position.y += 1;
	}

	if (input == DOWN && type == true)
	{
		create_mode_entity->position.y -= 1;
	}

	if (input == ACTION && type == true)
	{
		entities->push_back(create_mode_entity);

		render_target new_entity;
		new_entity.type = TYPE_ENTITY;
		create_mode_entity->position = create_mode_entity->position;
		new_entity.the_entity = create_mode_entity;
		render_targets->push_back(new_entity);

		current_entity = entities->size() - 1;
		editor_mode = EDIT_MODE;
		create_mode_entity = new Entity(t_vertex(0, 0, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 0));
	}
}

void LevelEditor::draw()
{
	if (editor_mode == CREATE_MODE)
	{
		glPushMatrix();
			create_mode_entity->draw();
		glPopMatrix();
	}
}