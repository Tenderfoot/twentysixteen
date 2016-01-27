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
	t_3dModel *test_chamber;
	float rotation;

	GLenum shader_program;
};