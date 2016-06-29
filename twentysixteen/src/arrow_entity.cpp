
#include "arrow.h"

void ArrowEntity::update(float time_delta)
{
	if (loosed)
		position.x -= (0.025)*time_delta;
}
