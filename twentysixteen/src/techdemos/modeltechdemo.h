#pragma once

#include "basetechdemo.h"
#include "../model_data.h"

class ModelTechDemo : public BaseTechDemo
{
public:
	ModelTechDemo()
	{
		this->techdemo_title = "Model Tech Demo";
	}

	void run();
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	t_3dModel *mymodel;

	// SOIL test
	GLuint texture;
};