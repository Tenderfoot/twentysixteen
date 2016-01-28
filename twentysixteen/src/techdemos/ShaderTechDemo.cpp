
#include "shadertechdemo.h"

// k talk this out
// the way I want the shader demo to go
// is you see a menu with two options:
// modify:
// room shader
// cube shader
// when you select one, the shader list will come up
// phong
// invert
// green
// When you select a shader, it will apply that shader to that model,
// and reset to the object menu

// how I can make that happen
// store the shader being used on ModelData
// use that when drawing
// create the listitem and set up keyboard control for them
// make it so that selecting shaders applies them to that model

void ShaderTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Shader Test Chamber", 0.5, 0.1, 0.3, 0.1));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));

	item_menu = new ListWidget({ "Room", "Cube" });
	TechDemoUI.add_widget(item_menu);
	item_menu->set_data(0.2, 0.5, 0.1, 0.05, true);

	shader_menu = new ListWidget({ "None", "Phong", "Inverter", "Green" });
	TechDemoUI.add_widget(shader_menu);
	shader_menu->set_data(0.2, 0.5, 0.1, 0.05, false);

	test_chamber.model = ModelData::import("testchamber.fbx", 0.05);
	test_chamber.shader = -1;

	current_list = item_menu;
}

void ShaderTechDemo::run(float time_delta)
{

	rotation += (time_delta / 10);
}

void ShaderTechDemo::take_input(boundinput input, bool type)
{
	if (type == true)
	{
		if (input == UP)
		{
			current_list->previous_item();
		}

		if (input == DOWN)
		{
			current_list->next_item();
		}

		if (input == ACTION)
		{
			if (current_list == item_menu)
			{
				chosen_model = current_list->list_items[current_list->current_selection];
				current_list->visible = false;
				current_list = shader_menu;
				current_list->visible = true;
			}
			else
			{
				char *chosen_shader = current_list->list_items[current_list->current_selection];
				
				current_list->visible = false;
				current_list = item_menu;
				current_list->visible = true;

				GLenum shader_to_apply;

				if (strcmp(chosen_shader, "None") == 0)
				{
					shader_to_apply = 0;
				}
				else if (strcmp(chosen_shader, "Phong") == 0)
				{
					shader_to_apply = Paintbrush::load_shader("phong");
				}
				else if (strcmp(chosen_shader, "Inverter") == 0)
				{
					shader_to_apply = Paintbrush::load_shader("inverter");
				}
				else if (strcmp(chosen_shader, "Green") == 0)
				{
					shader_to_apply = Paintbrush::load_shader("green");
				}

				if (strcmp(chosen_model, "Room") == 0)
				{
					test_chamber.shader = shader_to_apply;
				}
				else
				{
					cube_program = shader_to_apply;
				}

			}
		}
	}

	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void ShaderTechDemo::draw()
{
	gluLookAt(sin(rotation/100)*3, cos(rotation / 250) * 3,0,0,0,-25,0,1,0);

	glPushMatrix();
		glTranslatef(0.0f, -5.0f, -50.0f);
		Paintbrush::use_shader(test_chamber.shader);
		Paintbrush::draw_model(test_chamber.model);
		Paintbrush::stop_shader();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -22.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		glRotatef(rotation, 1.0f, 1.0f, 0.0f);
		Paintbrush::use_shader(cube_program);
		Paintbrush::draw_cube();
		Paintbrush::stop_shader();
	glPopMatrix();

	BaseTechDemo::draw();
}
