
#include "particles.h"

void ParticleEmitter::init(GLuint texture)
{
	int i;
		
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, t_vertex(0,0,0), t_vertex(3,2,0));
	}

}

void ParticleEmitter::init(GLuint texture, t_vertex position, t_vertex size)
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, position, size);
	}
}


void ParticleEmitter::reset()
{
}

void ParticleEmitter::update(float time_delta)
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->update(time_delta);
	}
}