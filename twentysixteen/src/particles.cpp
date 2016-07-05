
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

void ParticleEmitter::add_particle()
{
	if (particle_type == SIDE_FIRE)
	{
		int i;
		for (i = 0; i < 10; i++)
		{
			particles.push_back(new SideFireParticle);
		}
	}

	reset();
}

void ParticleEmitter::init(GLuint texture, t_vertex position, t_vertex size, particle_types type, int num_particles, bool prop)
{
	int i;
	this->position = position;
	this->size = size;
	this->initial_position = position;
	this->prop = prop;
	this->texture = texture;
	this->particle_type = type;

	if (type == STAR)
	{
		for (i = 0; i < num_particles; i++)
		{
			particles.push_back(new Star);
		}
	}
	else if (type == FIRE)
	{
		for (i = 0; i < num_particles; i++)
		{
			particles.push_back(new FireParticle);
		}
	}
	else if (type == POOF)
	{
		for (i = 0; i < num_particles; i++)
		{
			particles.push_back(new PoofParticle);
		}
	}
	else if (type == SIDE_FIRE)
	{
		for (i = 0; i < num_particles; i++)
		{
			particles.push_back(new SideFireParticle);
		}
	}

	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, position, size);
	}
}

void ParticleEmitter::reset()
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, position, size);
	}
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

void ParticleEmitter::update_position(t_vertex new_pos)
{
	int i;
	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->emission_position = new_pos;
	}
}

void ParticleEmitter::change_particle_type(particle_types new_type)
{
	particle_type = new_type;
	int particle_size;
	particle_size = particles.size();
	// this is a memory leak but i'm feeling lazy
	// will fix later
	particles.clear();

	int i;
	if (new_type == STAR)
	{
		for (i = 0; i < particle_size; i++)
		{
			particles.push_back(new Star);
		}
	}
	else if (new_type == FIRE)
	{
		for (i = 0; i < particle_size; i++)
		{
			particles.push_back(new FireParticle);
		}
	}
	else if (new_type == POOF)
	{
		for (i = 0; i < particle_size; i++)
		{
			particles.push_back(new PoofParticle);
		}
	}
	else if (new_type == SIDE_FIRE)
	{
		for (i = 0; i < particle_size; i++)
		{
			particles.push_back(new SideFireParticle);
		}
	}

	for (i = 0; i < particles.size(); i++)
	{
		particles.at(i)->init(texture, position, size);
	}
}