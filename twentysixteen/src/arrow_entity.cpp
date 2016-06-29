
#include "arrow.h"

void ArrowEntity::update(float delta_time)
{
	if (loosed)
		position.x -= (0.025)*delta_time;
}
