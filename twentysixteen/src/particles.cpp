
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
	this->position = position;
	this->size = size;
	this->initial_position = position;
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
		if (particles.at(i)->life < 0)
		{
			delete particles.at(i);
			particles.erase(particles.begin() + i);
		}
	}
}
void ParticleEmitter::draw()
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->draw();
	}
}

void ParticleEmitter::kill()
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->dying = true;
	}
}