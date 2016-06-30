
#include "arrow.h"
#include "player_entity.h"

void ArrowEntity::update(float time_delta)
{
	if (loosed)
	{
		position.x += velocity.x*time_delta;
		position.y += velocity.y*time_delta;

		t_vertex delta = t_vertex(player_pos.x - position.x, player_pos.y-3 - position.y, 0);
		delta.Normalize();

		velocity.x += (delta.x / 5000)*time_delta;
		velocity.y += (delta.y / 5000)*time_delta;
	}

	int j;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;

			if (((GameEntity*)game_entities->at(j))->check_against_game_entity(this))
			{
				if (((PlayerEntity*)game_entities->at(j))->state != DEAD)
				{
					((PlayerEntity*)game_entities->at(j))->state = DEAD;
					((PlayerEntity*)game_entities->at(j))->spine_data.start_time = SDL_GetTicks();
					if (((PlayerEntity*)game_entities->at(j))->staff_emitter != NULL)
					{
						((PlayerEntity*)game_entities->at(j))->staff_emitter->kill();
					}
				}
			}
		}
	}
}



void ArrowEntity::draw()
{
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);

		float angle;
		angle = tan(velocity.y / velocity.x) * 57.29;
		printf("angle was %f\n", angle);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glScalef(size.x, size.y, size.z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(color.x, color.y, color.z);
		Paintbrush::draw_quad();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
