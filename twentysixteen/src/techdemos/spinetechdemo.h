#pragma once

#include "basetechdemo.h"
#include "../spine_data.h"

class SpineTechDemo : public BaseTechDemo
{
	void run();
	void draw();
	void init();

	// SOIL test
	SpineData spineboy;
};