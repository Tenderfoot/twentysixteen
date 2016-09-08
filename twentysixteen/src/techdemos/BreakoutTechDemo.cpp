
#include "breakouttechdemo.h"


void BreakoutTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("PRESS SPACE TO START", 0.5, 0.05, 0.5, 0.05));
	spineboy.load_spine_data("willy");
	spineboy.animation_name = "mouth_open";
	spineboy.looping = false;


	while (myemitter.particles.size() < 50)
	{
		myemitter.particles.push_back(new BallFireParticle);
	}
	
	myemitter.init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(0, 0, 0), t_vertex(1, 1, 0));

	entities.push_back(&myemitter);

	// ball
	ball.size = t_vertex(1.5, 1.5, 0);
	ball.position = t_vertex(0, 0, 0);
	ball.velocity = t_vertex(0, 0, 0);

	// paddle;
	paddle.size = t_vertex(2, 0.25, 0);
	paddle.position = t_vertex(0, -6, 0);

	int i,j;
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			int brick_index = (i * 5) + j;
			bricks.push_back(GameEntity());
			bricks.at(brick_index).size = t_vertex(1.0f, 0.5f, 0.0f);

			if(j%2 == 0)
				bricks.at(brick_index).position = t_vertex(-6 + (i * 3), (2+j), 0);
			else
				bricks.at(brick_index).position = t_vertex(-4.5 + (i * 3), (2 + j), 0);
			bricks.at(brick_index).activated = false;
		}
	}
}

void BreakoutTechDemo::run(float time_delta)
{
	spineboy.update_skeleton(time_delta);
	int i;

	for (i = 0; i < entities.size(); i++)
	{
		if (entities.at(i)->type == EMITTER_ENTITY)
		{
			((ParticleEmitter*)entities.at(i))->update(time_delta);
		}
	}

	// determine paddle velocity...

	if (move_right && !move_left)
	{
		paddle.velocity.x = 7.5;
	}
	else if (move_left && !move_right)
	{
		paddle.velocity.x = -7.5;
	}
	else
	{
		paddle.velocity.x = 0;
	}
	ParticleEmitter *poof_emitter;
	// brick collision
	for (i = 0; i < bricks.size(); i++)
	{
		if (bricks.at(i).activated == false)
		{
			if (LinearAlgebra::PolygonCollision(ball.return_polygon(), bricks.at(i).return_polygon(), t_vertex(ball.velocity.x*time_delta*0.001, 0, 0)).WillIntersect == true)
			{
				ball.velocity.x = -ball.velocity.x;
				bricks.at(i).activated = true;
				spineboy.start_time = SDL_GetTicks();

				// &&&&&&&&&&&&
				poof_emitter = new ParticleEmitter();
				while (poof_emitter->particles.size() < 50)
				{
					poof_emitter->particles.push_back(new PoofParticle);
				}

				poof_emitter->init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(bricks.at(i).position.x, bricks.at(i).position.y, 0), t_vertex(1, 1, 0));

				entities.push_back(poof_emitter);


			}
			if (LinearAlgebra::PolygonCollision(ball.return_polygon(), bricks.at(i).return_polygon(), t_vertex(0, ball.velocity.y*time_delta*0.001, 0)).WillIntersect == true)
			{
				ball.velocity.y = -ball.velocity.y;
				bricks.at(i).activated = true;
				spineboy.start_time = SDL_GetTicks();

				// &&&&&&&&&&&&
				poof_emitter = new ParticleEmitter();
				while (poof_emitter->particles.size() < 50)
				{
					poof_emitter->particles.push_back(new PoofParticle);
				}

				poof_emitter->init(Paintbrush::get_texture("data/images/fire.png", false, false), t_vertex(bricks.at(i).position.x, bricks.at(i).position.y, 0), t_vertex(1, 1, 0));

				entities.push_back(poof_emitter);
			}
		}
	}

	// paddle collision
	if (LinearAlgebra::PolygonCollision(ball.return_polygon(), paddle.return_polygon(), t_vertex(-(paddle.velocity.x*time_delta*0.001)+ball.velocity.x*time_delta*0.001, 0, 0)).WillIntersect == true)
	{
		ball.velocity.x = -ball.velocity.x;
		spineboy.start_time = SDL_GetTicks();
	}
	if (LinearAlgebra::PolygonCollision(ball.return_polygon(), paddle.return_polygon(), t_vertex(0, ball.velocity.y*time_delta*0.001, 0)).WillIntersect == true)
	{
		ball.velocity.y = -ball.velocity.y;
		spineboy.start_time = SDL_GetTicks();
	}

	/************ Ball stuff, would normally be in entity specific file *************/
//	ball.position += ball.velocity*time_delta;
	ball.position.x += ball.velocity.x*time_delta*0.001;
	ball.position.y += ball.velocity.y*time_delta*0.001;
	paddle.position.x += paddle.velocity.x*time_delta*0.001;
	
	if (ball.position.x < -10 + (ball.size.x / 2))
	{
		ball.position.x = -10 + (ball.size.x / 2);
		ball.velocity.x = -ball.velocity.x;
		spineboy.start_time = SDL_GetTicks();
	}

	if (ball.position.x > 10 - (ball.size.x / 2))
	{
		ball.position.x = 10 - (ball.size.x / 2);
		ball.velocity.x = -ball.velocity.x;
		spineboy.start_time = SDL_GetTicks();
	}

	if (ball.position.y < -8 + (ball.size.y / 2))
	{
		ball.position.y = -8 + (ball.size.y / 2);
		ball.velocity.y = -ball.velocity.y;
		spineboy.start_time = SDL_GetTicks();
	}

	if (ball.position.y > 7 - (ball.size.y / 2))
	{
		ball.position.y = 7 - (ball.size.y / 2);
		ball.velocity.y = -ball.velocity.y;
		spineboy.start_time = SDL_GetTicks();
	}

	myemitter.update_position(ball.position);
}

void BreakoutTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = QUIT;

	if (input == LEFT)
	{
		move_left = type;
	}

	if (input == RIGHT)
	{
		move_right = type;
	}

	if (input == ACTION && type == true)
	{
		if (start == false)
		{
			start = true;
			ball.velocity = t_vertex(7, 7, 0);
			TechDemoUI.widgets.clear();
		}
	}
}

void BreakoutTechDemo::draw()
{
	BaseTechDemo::draw();

	int i;
	for (i = 0; i < bricks.size(); i++)
	{
		glPushMatrix();
		if (bricks.at(i).activated == false)
		{
			glTranslatef(bricks.at(i).position.x, bricks.at(i).position.y, -10.0f);
			glScalef(bricks.at(i).size.x, bricks.at(i).size.y, 1.0f);
			glColor3f((((float)i) / bricks.size()), ((float)((SDL_GetTicks())%1200))/1200, ((float)((SDL_GetTicks()) % 600)) / 600);
			glDisable(GL_TEXTURE_2D);
			Paintbrush::draw_quad();
			glEnable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}

	int j;

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -10.0f);
		for (j = 0; j < entities.size(); j++)
		{
			for (i = 0; i < ((ParticleEmitter*)entities.at(j))->particles.size(); i++)
			{
				((ParticleEmitter*)entities.at(j))->particles.at(i)->draw();

				if (SDL_GetTicks() - ((ParticleEmitter*)entities.at(j))->start_time > 100 && j != 0)
				{
					((ParticleEmitter*)entities.at(j))->kill();
				}
			}
		}
	glPopMatrix();


	glPushMatrix();
		glTranslatef(paddle.position.x, paddle.position.y, -10.0f);
		glScalef(paddle.size.x, paddle.size.y, 1.0f);
		glColor3f(1,1,1);
		glDisable(GL_TEXTURE_2D);
		Paintbrush::draw_quad();
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(ball.position.x, ball.position.y, -10.0f);
		glScalef(ball.size.x, ball.size.y, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
			glTranslatef(0.0f, -0.2f, 0.0f);
			glScalef(0.002f, 0.0019f, 0.002f);
			//spineboy.draw();
		glPopMatrix();
	glPopMatrix();
}
