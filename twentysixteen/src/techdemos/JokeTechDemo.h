#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"

class JokeTechDemo : public BaseTechDemo
{
public:
	JokeTechDemo()
	{
		this->techdemo_title = "Spine Tech Demo";
	}

	void run(float time_delta);
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	// SOIL test
	SpineData spineboy;
};