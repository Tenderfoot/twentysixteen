#pragma once

#include "basetechdemo.h"

class TTFTechDemo : public BaseTechDemo
{
public:
	TTFTechDemo()
	{
		this->techdemo_title = "TTF Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	// SOIL test
	GLuint texture;
};