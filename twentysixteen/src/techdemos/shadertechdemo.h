#pragma once

#include "basetechdemo.h"

class ShaderTechDemo : public BaseTechDemo
{
public:
	ShaderTechDemo()
	{
		this->techdemo_title = "TTF Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	// SOIL test
	GLuint texture;
	ModelData test_chamber;
	float rotation;
	GLenum cube_program;

	ListWidget *item_menu;
	ListWidget *shader_menu;
	ListWidget *current_list;

	char *chosen_model;
};