
#include "particles.h"

void ParticleEmitter::init(GLuint texture)
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, t_vertex(0,0,0), t_vertex(5,2,0));
	}
}

void ParticleEmitter::reset()
{
}
