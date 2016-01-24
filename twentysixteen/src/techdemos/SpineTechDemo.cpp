
#include "spinetechdemo.h"

void SpineTechDemo::init()
{
	spineboy.load_spine_data("spineboy");

}

void SpineTechDemo::run()
{
	spineboy.update_skeleton();
}

void SpineTechDemo::draw()
{
	BaseTechDemo::draw();
	spineboy.draw();
}
