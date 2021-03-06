
#include "archer_entity.h"

void ArcherEntity::update(float time_delta)
{
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;
}

void ArcherEntity::init()
{
	spine_data.load_spine_data("everybody");
	spSkeleton_setSkinByName(spine_data.skeleton, "knight");
	spine_data.animation_name = "shoot";
	spine_data.start_time = SDL_GetTicks();

	current_arrow = NULL;
}


void ArcherEntity::player_update(float time_delta)
{
	spine_data.update_skeleton(time_delta);

	int j;
	for (j = 0; j < game_entities->size(); j++)
	{
		if (game_entities->at(j)->type == PLAYER_ENTITY)
		{
			player_pos = game_entities->at(j)->position;
		}
	}

	if (player_pos.x > position.x - 30 && player_pos.x < position.x + 30)
	{
		if (SDL_GetTicks() - spine_data.start_time > 700)
		{
			// loose this arrow
			if (current_arrow != NULL)
				current_arrow->loosed = true;

			current_arrow = NULL;
		}

		if (SDL_GetTicks() - spine_data.start_time > 1000)
		{
			spine_data.start_time = SDL_GetTicks();

			// queue another one
			current_arrow = new ArrowEntity();
			current_arrow->position = position;
			current_arrow->position.z += 0.5;
			current_arrow->game_entities = game_entities;
			current_arrow->create_time = SDL_GetTicks();
			add_entity(current_arrow);
		}

		t_vertex emit_pos = spine_data.get_slot_location("left_hand");
		emit_pos.x += position.x;

		if (spine_data.flip)
			emit_pos.x += 0.5;
		else
			emit_pos.x -= 0.5;

		emit_pos.y += position.y + 0.1;

		if (current_arrow != NULL)
			current_arrow->position = emit_pos;
	}

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.001;
	}

}