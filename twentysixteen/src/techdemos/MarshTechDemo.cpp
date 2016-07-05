
#include "marshtechdemo.h"

void MarshTechDemo::init()
{
	init_level("crem2");

	side_fire = new ParticleEmitter();

	int i;
	for (i = 0; i < 25; i++)
		side_fire->particles.push_back(new SideFireParticle);

	side_fire->position = t_vertex(15,0,0);
	side_fire->size = t_vertex(5.0f, 5.0f, 1.0f);

	entities.push_back(side_fire);

	build_render_targets();
}
