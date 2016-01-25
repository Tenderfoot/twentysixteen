#pragma once

#include "basetechdemo.h"


class AudioTechDemo : public BaseTechDemo
{
public:
	AudioTechDemo()
	{
		this->techdemo_title = "Audio Tech Demo";
	}

	void run();
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	// SOIL test
	GLuint texture;
};