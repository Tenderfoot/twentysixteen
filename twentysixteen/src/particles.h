#pragma once

#include <SDL_opengl.h>

#include <spine/spine.h>
#include <spine/extension.h>

#include <vector>

#include "common.h"
#include "entity.h"

class Particle : public Entity
{
public:
	Particle(t_vertex position, t_vertex velocity): position(position), velocity(velocity)
	{
	}

	Particle()
	{
	}

	void reset()
	{
	}

	virtual void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size) = 0;

	t_vertex position;
	t_vertex velocity;
	t_vertex size;

	// emitter stuff...
	t_vertex emission_position;
	t_vertex emission_size;
	
	GLuint texture;
	float r, g, b;
	float life;
	bool dir;
};

class ParticleEmitter
{
public:
	void init(GLuint texture);
	void init(GLuint texture, t_vertex position, t_vertex size);
	void reset();
	void update(float time_delta);
	std::vector<Particle*> particles;
};

// Fire Particle
class FireParticle : public Particle
{
public:

	void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size)
	{
		texture = particle_texture;

		this->emission_position = emission_position;
		this->emission_size = emission_size;

		reset();
	}

	virtual void reset()
	{
		life = rand() % 200;

		float x;
		x = rand() % 100;
		x = x / 100;

		position.x = emission_size.x*x + emission_position.x;

		x = (rand() % 250) + 100;
		x = x / 100;

		size.x = x;

		x = rand() % 100;
		x = x / 100;

		position.y = emission_size.y*x + emission_position.y;
	}

	virtual void update(float time_delta)
	{
		life = life - 1*(time_delta/5);
		position.y += 0.005*time_delta;

		if (life < 0)
			reset();
	}

	void draw()
	{
		glPushMatrix();

			// bind texture and setup
			glBindTexture(GL_TEXTURE_2D, texture);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glColor4f(1.0f, (100 - life) / 80, 0.0f, life / 100);

			// transform
			glTranslatef(position.x, position.y, 0.0f);
			glScalef(size.x, size.x, 0.0f);

			// draw
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.0f);
			glEnd();

			// reset
			glColor3f(1.0f, 1.0f, 1.0f);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

		glPopMatrix();
	}
};

// Fire Particle
class Star : public Particle
{
public:

	void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size)
	{
		texture = particle_texture;

		this->emission_position = emission_position;
		this->emission_size = emission_size;

		reset();
	}

	virtual void reset()
	{
		dir = false;

		life = rand() % 100;

		float x;
		x = rand() % 100;
		x = x / 100;

		position.x = emission_size.x*x + emission_position.x;

		x = rand() % 100;
		x = x / 100;

		size.x = x;

		x = rand() % 100;
		x = x / 100;

		position.y = emission_size.y*x + emission_position.y;
	}

	virtual void update(float time_delta)
	{
		if(dir)
			life = life + 1 * (time_delta / 10);
		else
			life = life - 1 * (time_delta / 10);

		if (life < 0 || life > 100)
			dir = !dir;
	}

	void draw()
	{
		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0, 1.0, life / 100);

		// transform
		glTranslatef(position.x, position.y, 0.0f);
		glScalef(size.x, size.x, 0.0f);

		// draw
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();

		// reset
		glColor3f(1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glPopMatrix();
	}
};


