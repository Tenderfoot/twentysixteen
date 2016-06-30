
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

		build_ui();
	}

	if (input == EDITOR_EDIT_MODE && type == true)
	{
		if (editor_mode == PLAY_MODE)
		{
			reset_entities();
		}
		editor_mode = EDIT_MODE;

		build_ui();
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
		if (entities->at(i)->type == PLAYER_ENTITY)
		{
			((PlayerEntity*)entities->at(i))->state = IDLE;
			((PlayerEntity*)entities->at(i))->spine_data.looping = true;
		}
		if (entities->at(i)->type == EMITTER_ENTITY)
		{
			((ParticleEmitter*)entities->at(i))->kill();
		}


		if (entities->at(i)->type == ARROW_ENTITY || entities->at(i)->type == SKELETON_ENTITY)
		{
			remove_entity_at_index(i);
			i--;
		}

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
	this->level_name = level_name;

	std::stringstream filename;
	filename << "data/levels/" << level_name.c_str() <<  "/" << level_name.c_str() << ".txt";
	std::ifstream in(filename.str());
	std::string line;

	Entity *new_entity;
	GLuint texture;

	// get number of entities
	std::getline(in, line);
	int number = std::stoi(line);
	
	t_vertex new_pos, new_size, new_color;

	while (std::getline(in, line))
	{
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

			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new GameEntity(new_pos, new_size, t_vertex(1.0f,1.0f,1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = texture;

			entities->push_back(new_entity);
		}
		if (line == "PortcullisEntity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new PortcullisEntity(new_pos, new_size, t_vertex(1.0f, 1.0f, 1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = Paintbrush::get_texture("data/images/metal_gate.png", false, true);

			entities->push_back(new_entity);
		}
		if (line == "ButtonEntity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new ButtonEntity(new_pos, new_size, t_vertex(1.0f, 1.0f, 1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = texture;

			entities->push_back(new_entity);
		}
		if (line == "ArcherEntity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new ArcherEntity(new_pos, new_size, t_vertex(1.0f, 1.0f, 1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = texture;

			((ArcherEntity*)new_entity)->init();
			((ArcherEntity*)new_entity)->spine_data.setslots();

			entities->push_back(new_entity);
		}
		if (line == "SwordsmanEntity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);

			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new SwordsmanEntity(new_pos, new_size, t_vertex(1.0f, 1.0f, 1.0f));

			new_entity->color = t_vertex(1.0f, 1.0f, 1.0f);
			new_entity->texture = texture;

			((SwordsmanEntity*)new_entity)->init();
			((SwordsmanEntity*)new_entity)->spine_data.setslots();

			entities->push_back(new_entity);
		}
		if (line == "Entity")
		{
			new_pos = get_vertex_from_buffer(&in);
			new_size = get_vertex_from_buffer(&in);
			new_color = get_vertex_from_buffer(&in);
			
			std::getline(in, line, ',');
			texture = std::stoi(line);

			new_entity = new Entity(new_pos, new_size, new_color, texture);

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
	std::stringstream filename;
	filename << "data/levels/" << level_name.c_str()  << "/" << level_name.c_str() << ".txt";
	myfile.open(filename.str());
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
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
		if (entities->at(i)->type == ENTITY)
		{
			myfile << "Entity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->color.x << "," << entities->at(i)->color.y << "," << entities->at(i)->color.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
		if (entities->at(i)->type == EMITTER_ENTITY)
		{
			myfile << "EmitterEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << "," << "\n";
		}
		if (entities->at(i)->type == PORTCULLIS_ENTITY)
		{
			myfile << "PortcullisEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
		if (entities->at(i)->type == BUTTON_ENTITY)
		{
			myfile << "ButtonEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
		if (entities->at(i)->type == ARCHER_ENTITY)
		{
			myfile << "ArcherEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
		if (entities->at(i)->type == SWORDSMAN_ENTITY)
		{
			myfile << "SwordsmanEntity\n";
			myfile << entities->at(i)->initial_position.x << "," << entities->at(i)->initial_position.y << "," << entities->at(i)->initial_position.z << ",";
			myfile << entities->at(i)->size.x << "," << entities->at(i)->size.y << "," << entities->at(i)->size.z << ",";
			myfile << entities->at(i)->texture << "," << "\n";
		}
	}

	myfile.close();
}

void LevelEditor::remove_entity_at_index(int index)
{
	int i;
	for (i = 0; i < render_targets->size(); i++)
	{
		if (entities->at(index) == render_targets->at(i).the_entity)
		{
			render_targets->erase(render_targets->begin() + i);
		}
	}

	delete entities->at(index);
	entities->erase(entities->begin() + index);
}

void LevelEditor::input_edit(boundinput input, bool type)
{
	if (input == EDITOR_DELETE && type == true)
	{	
		remove_entity_at_index(current_entity);
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
		build_ui();
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
		build_ui();
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

	if (input == QUICKSWITCH && type == true)
	{
		entities->at(current_entity)->position.z -= 1;
	}

	if (input == USE && type == true)
	{
		entities->at(current_entity)->position.z += 1;
	}

	if (input == EDITOR_RED && type == true)
	{
		entities->at(current_entity)->color.x = (entities->at(current_entity)->color.x + 0.05f);
		if (entities->at(current_entity)->color.x > 1)
			entities->at(current_entity)->color.x = 0;
	}

	if (input == EDITOR_BLUE && type == true)
	{
		entities->at(current_entity)->color.y = (entities->at(current_entity)->color.y + 0.05f);
		if (entities->at(current_entity)->color.y > 1)
			entities->at(current_entity)->color.y = 0;
	}

	if (input == EDITOR_GREEN && type == true)
	{
		entities->at(current_entity)->color.z = (entities->at(current_entity)->color.z + 0.05f);
		if (entities->at(current_entity)->color.z > 1)
			entities->at(current_entity)->color.z = 0;
	}

	if (input == EDITOR_SCALE_X_PLUS && type == true)
	{
		entities->at(current_entity)->size.x = (entities->at(current_entity)->size.x + 0.05f);
		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_SCALE_Y_PLUS && type == true)
	{
		entities->at(current_entity)->size.y = (entities->at(current_entity)->size.y + 0.05f);
		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_SCALE_Z_PLUS && type == true)
	{
		entities->at(current_entity)->size.z = (entities->at(current_entity)->size.z + 0.05f);
		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_SCALE_X_MINUS && type == true)
	{
		entities->at(current_entity)->size.x = (entities->at(current_entity)->size.x - 0.05f);
		if (entities->at(current_entity)->size.x < 0)
			entities->at(current_entity)->size.x = 0;

		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_SCALE_Y_MINUS && type == true)
	{
		entities->at(current_entity)->size.y = (entities->at(current_entity)->size.y - 0.05f);
		if (entities->at(current_entity)->size.y < 0)
			entities->at(current_entity)->size.y = 0;

		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_SCALE_Z_MINUS && type == true)
	{
		entities->at(current_entity)->size.z = (entities->at(current_entity)->size.z - 0.05f);
		if (entities->at(current_entity)->size.z < 0)
			entities->at(current_entity)->size.z = 0;

		entities->at(current_entity)->initial_size = entities->at(current_entity)->size;
	}

	if (input == EDITOR_T && type == true)
	{
		if (entities->at(current_entity)->type == ENTITY || entities->at(current_entity)->type == GAME_ENTITY)
		{
			entities->at(current_entity)->texture = (entities->at(current_entity)->texture + 1) % Paintbrush::texture_db.size();
		}
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
			create_mode_entity = new Entity(t_vertex(pos.x, pos.y, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 0), NULL);
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
		case ARCHER_ENTITY:
			create_mode_entity = new ArcherEntity(t_vertex(pos.x, pos.y, 0), t_vertex(1, 3, 1), t_vertex(1, 0, 1));
			((ArcherEntity*)create_mode_entity)->game_entities = entities;
			((ArcherEntity*)create_mode_entity)->init();
			((ArcherEntity*)create_mode_entity)->position = pos;
			((ArcherEntity*)create_mode_entity)->spine_data.setslots();
			break;
		case BUTTON_ENTITY:
			create_mode_entity = new ButtonEntity(t_vertex(pos.x, pos.y, 0), t_vertex(4, 2, 1), t_vertex(1, 1, 1));
			break;
		case PORTCULLIS_ENTITY:
			create_mode_entity = new PortcullisEntity(t_vertex(pos.x, pos.y, 0), t_vertex(1, 2, 1), t_vertex(1, 1, 1));
			break;
		case SWORDSMAN_ENTITY:
			create_mode_entity = new SwordsmanEntity(t_vertex(pos.x, pos.y, 0), t_vertex(1, 3, 1), t_vertex(1, 0, 1));
			((SwordsmanEntity*)create_mode_entity)->game_entities = entities;
			((SwordsmanEntity*)create_mode_entity)->init();
			((SwordsmanEntity*)create_mode_entity)->position = pos;
			((SwordsmanEntity*)create_mode_entity)->spine_data.setslots();
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
		current_type = (current_type + 1) % NUM_TOTAL_ENTITIES;
		build_entity();
		build_ui();
	}

	if (input == PREVIOUS && type == true)
	{
		current_type = (current_type - 1) % NUM_TOTAL_ENTITIES;
		build_entity();
		build_ui();
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

	if (input == QUICKSWITCH && type == true)
	{
		create_mode_entity->position.z -= 1;
	}

	if (input == USE && type == true)
	{
		create_mode_entity->position.z += 1;
	}

	if (input == ACTION && type == true)
	{
		entities->push_back(create_mode_entity);
		current_entity = entities->size() - 1;
		editor_mode = EDIT_MODE;
		create_mode_entity = new Entity(t_vertex(0, 0, 0), t_vertex(5, 5, 5), t_vertex(1, 0, 0));

		build_ui();
	}
}

void LevelEditor::build_ui()
{
	int i;
	for (i = 0; i < editor_interface.widgets.size(); i++)
	{
		delete editor_interface.widgets.at(i);
	}

	editor_interface.widgets.clear();
	
	editor_interface.add_widget(new UIImage(0, 0, 0.5, 2, NULL));

	editor_interface.add_widget(new TextWidget("Level Editor", 0.125, 0.025, 0.25, 0.05));
	if (editor_mode == CREATE_MODE)
	{
		editor_interface.add_widget(new TextWidget("Create Mode", 0.075, 0.075, 0.15, 0.035));
		editor_interface.add_widget(new TextWidget("Entity Type:", 0.075, 0.115, 0.15, 0.035));
		editor_interface.add_widget(new TextWidget(entity_stringname[create_mode_entity->type], 0.075, 0.15, 0.10, 0.03));
	}
	if (editor_mode == EDIT_MODE)
	{
		editor_interface.add_widget(new TextWidget("Edit Mode", 0.075, 0.075, 0.15, 0.035));
		editor_interface.add_widget(new TextWidget("Entity Type:", 0.075, 0.115, 0.15, 0.035));
		editor_interface.add_widget(new TextWidget(entity_stringname[entities->at(current_entity)->type], 0.075, 0.15, 0.10, 0.03));
	}

	editor_interface.add_widget(new TextWidget("F1 - Create Mode", 0.05, 0.3, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("F2 - Edit Mode", 0.05, 0.32, 0.1, 0.02));

	editor_interface.add_widget(new TextWidget("W - Move Up", 0.05, 0.34, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("S - Move Down", 0.05, 0.36, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("A - Move left", 0.05, 0.38, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("D - Move Right", 0.05, 0.4, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("Q - Move Forward", 0.05, 0.42, 0.1, 0.02));
	editor_interface.add_widget(new TextWidget("E - Move Back", 0.05, 0.44, 0.1, 0.02));

	if (editor_mode == CREATE_MODE)
	{
		editor_interface.add_widget(new TextWidget("Page Up - Next Type", 0.075, 0.46, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Page Down - Previous Type", 0.075, 0.48, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Space - Create Entity", 0.075, 0.5, 0.15, 0.02));
	}
	if (editor_mode == EDIT_MODE)
	{
		editor_interface.add_widget(new TextWidget("Page Up - Next Entity", 0.075, 0.46, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Page Down - Previous Entity", 0.075, 0.48, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Delete - Delete Entity", 0.075, 0.5, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Num 7/4 - Scale X", 0.075, 0.52, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Num 8/5 - Scale Y", 0.075, 0.54, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Num 9/6 - Scale Z", 0.075, 0.56, 0.15, 0.02));
		editor_interface.add_widget(new TextWidget("Num 1/2/4 - R/G/B", 0.075, 0.58, 0.15, 0.02));

		if (entities->at(current_entity)->type == ENTITY || entities->at(current_entity)->type == GAME_ENTITY)
		{
			editor_interface.add_widget(new TextWidget("T - Cycle Texture", 0.075, 0.6, 0.15, 0.02));
		}
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
	if (editor_mode == CREATE_MODE || editor_mode == EDIT_MODE)
	{
		glPushMatrix();
			editor_interface.draw();
		glPopMatrix();
	}
}