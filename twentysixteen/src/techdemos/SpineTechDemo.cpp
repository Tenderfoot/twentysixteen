
#include "spinetechdemo.h"

void SpineTechDemo::init()
{
	spineboy.load_spine_data("old");
}

void SpineTechDemo::run()
{
	spineboy.update_skeleton();
}

void SpineTechDemo::draw()
{
	spineboy.draw();
}
