#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"

class SpineTechDemo : public BaseTechDemo
{
public:
	SpineTechDemo()
	{
		this->techdemo_title = "Spine Tech Demo";
	}

	void run();
	void draw();
	void init();
	void take_input(boundinput input, bool type);

	// SOIL test
	SpineData spineboy;
};