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
	Particle(t_vertex position, t_vertex size, t_vertex velocity): position(position), velocity(velocity)
	{
	}

	Particle()
	{
		r = 0;
		g = 0;
		b = 0;
		life = 0;
		dir = false;
		position = t_vertex(0, 0, 0);
		velocity = t_vertex(0, 0, 0);
		size = t_vertex(0, 0, 0);
	}

	void reset()
	{
	}

	virtual void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size) = 0;

	virtual void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size, int max_life)
	{
		texture = particle_texture;

		this->emission_position = emission_position;
		this->emission_size = emission_size;

		this->max_life = max_life;

		reset();
	}

	t_vertex position;
	t_vertex velocity;
	t_vertex size;

	int max_life;

	// emitter stuff...
	t_vertex emission_position;
	t_vertex emission_size;
	
	GLuint texture;
	float r, g, b;
	float life;
	bool dir;
	bool dying;
};

class ParticleEmitter : public Entity
{
public:

	ParticleEmitter()
	{
		type = EMITTER_ENTITY;
		start_time = SDL_GetTicks();
	}

	particle_types particle_type;

	void init(GLuint texture);
	void init(GLuint texture, t_vertex position, t_vertex size);
	void init(GLuint texture, t_vertex position, t_vertex size, particle_types new_type, int num_particles, bool prop);
	void reset();
	void draw();
	void update(float time_delta);
	void kill();
	void change_particle_type(particle_types new_type);
	void change_max_life(int new_max_life)
	{
		int i;
		for (i = 0; i < particles.size(); i++)
		{
			particles.at(i)->max_life = new_max_life;
		}
	}
	void add_particle();

	// is this a dynamic effect or a prop?
	// clean up effects on reset, leave props;
	bool prop;

	int start_time;

	void update_position(t_vertex new_pos);

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
		max_life = 250;
		reset();
	}

	void reset()
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

		position.y = emission_position.y;
	}

	virtual void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		position.y += 0.005*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void draw()
	{

		glPushMatrix();

			// bind texture and setup
			glBindTexture(GL_TEXTURE_2D, texture);

			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);
			glColor4f(1.0f, (100 - life) / 80, 0.0f, life / 100);

			// transform
			glTranslatef(position.x, position.y, position.z);
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
			glDepthMask(GL_TRUE);

		glPopMatrix();
	}
};


// BallFire Particle
class BallFireParticle : public Particle
{
public:

	void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size)
	{
		texture = particle_texture;

		this->emission_position = emission_position;
		this->emission_size = emission_size;
		max_life = 250;
		reset();
	}

	void reset()
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

		position.y = emission_position.y;
	}

	virtual void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		//position.y += 0.005*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void draw()
	{

		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glColor4f(1.0f, (100 - life) / 80, 0.0f, life / 100);

		// transform
		glTranslatef(position.x, position.y, position.z);
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
		glDepthMask(GL_TRUE);

		glPopMatrix();
	}
};

// Fire Particle
class SideFireParticle : public FireParticle
{
public:

	void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		position.x -= 0.05*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void reset()
	{
		life = rand() % max_life;

		float x;
		x = rand() % 100;
		x = x / 100;

		position.y = emission_size.y*x + emission_position.y;

		x = (rand() % 500) + 250;
		x = x / 100;

		size.x = x;

		x = rand() % 100;
		x = x / 100;

		position.x = emission_position.x;
	}

	void draw()
	{

		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glColor4f(1.0f, (200 - life)/200, 0.0f, life / 100);

		// transform
		glTranslatef(position.x, position.y, position.z);
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
		glDepthMask(GL_TRUE);

		glPopMatrix();
	}
};

// Fire Particle
class DirtParticle : public Particle
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

		position.y = emission_position.y;
	}

	virtual void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		position.y += 0.005*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void draw()
	{

		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glColor4f(0.2f, 0.1f, 0.0f, life/100);

		// transform
		glTranslatef(position.x, position.y, position.z);
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
		glDepthMask(GL_TRUE);

		glPopMatrix();
	}
};

class PoofParticle : public Particle
{
public:

	void init(GLuint particle_texture, t_vertex emission_position, t_vertex emission_size)
	{
		texture = particle_texture;

		this->emission_position = emission_position;
		this->emission_size = emission_size;

		reset();
	}

	t_vertex particle_velocity;

	virtual void reset()
	{
		life = rand() % 200;

		float x;
		x = rand() % 100;
		x = x / 100;

		position.x = emission_size.x*x + emission_position.x;
		position.y = emission_size.y*x + emission_position.y;

		particle_velocity = t_vertex((((float)(rand() % 100))/100)*0.01, (((float)(rand() % 100)) / 100)*0.01, 0.0f);

		if (rand() % 2 == 0)
		{
			particle_velocity.x = -particle_velocity.x;
		}

		if (rand() % 2 == 0)
		{
			particle_velocity.y = -particle_velocity.y;
		}

		x = (rand() % 250) + 100;
		x = x / 100;

		size.x = x;



		x = rand() % 100;
		x = x / 100;
	}

	virtual void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		position.x += particle_velocity.x*time_delta;
		position.y += particle_velocity.y*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void draw()
	{

		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, life / 100);

		// transform
		glTranslatef(position.x-1.0f, position.y, position.z);
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
class StaffParticle : public Particle
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

		x = (rand() % 125) + 50;
		x = x / 100;

		size.x = x;

		x = rand() % 100;
		x = x / 100;

		position.y = emission_position.y;
	}

	virtual void update(float time_delta)
	{
		life = life - 1 * (time_delta / 5);
		position.y += 0.005*time_delta;

		if (life < 0 && dying == false)
			reset();
	}

	void draw()
	{

		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glColor4f(1.0f, (100 - life) / 80, 0.0f, life / 100);

		// transform
		glTranslatef(position.x, position.y, position.z);
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
		glDepthMask(GL_TRUE);

		glPopMatrix();
	}
};

// star Particle
class Star : public Particle
{
public:

	float speed;

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

		x = rand() % 200;
		x = x / 50;
		size.x = x;

		x = rand() % 100;
		x = x / 100;
		speed = x*10+10;

		x = rand() % 100;
		x = x / 100;

		position.y = emission_size.y*x + emission_position.y;

		position.z = emission_position.z;
	}

	virtual void update(float time_delta)
	{
		if(dir)
			life = life + 1 * (time_delta / speed);
		else
			life = life - 1 * (time_delta / speed);

		if (life < 0 || life > 100)
			dir = !dir;

		if (life < 0)
			life = 1;
	}

	void draw()
	{
		glPushMatrix();

		// bind texture and setup
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0, 1.0, life / 100);

		// transform
		glTranslatef(position.x-400, position.y, position.z);
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

		glPopMatrix();
	}
};


