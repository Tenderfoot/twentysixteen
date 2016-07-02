
#include "player_entity.h"
#include "skeleton_entity.h"

void PlayerEntity::handle_keypress(boundinput input, bool type)
{
	keydown_map[input] = type;

	if (input == HAT_CLEAR)
	{
		keydown_map[LEFT] = false;
		keydown_map[RIGHT] = false;
		keydown_map[UP] = false;
		keydown_map[DOWN] = false;
	}

	if (input == LEFT && type == true)
	{
		keydown_map[RIGHT] = false;
	}
	else if (input == RIGHT && type == true)
	{
		keydown_map[LEFT] = false;
	}


	if (input == EDITOR_T && type == true && velocity.y > -0.005 && velocity.y < 0.005 && state == IDLE)
	{
		SkeletonEntity *test = new SkeletonEntity();
		test->position = position;
		if (spine_data.flip)
			test->position.x += 5;
		else
			test->position.x -= 5;
		test->size = t_vertex(1, 2.8, 1);
		test->game_entities = game_entities;
		test->init("skel");
		add_entity(test);
		state = CASTING;
		spine_data.start_time = SDL_GetTicks();

		staff_emitter = new ParticleEmitter();
		int i;
		for (i = 0; i < 10; i++)
			staff_emitter->particles.push_back(new StaffParticle);

		t_vertex emit_pos = spine_data.get_slot_location("staff_tip");
		emit_pos.x += position.x;
		emit_pos.y += position.y;

		staff_emitter->init(Paintbrush::get_texture("data/images/fire.png", false, false), emit_pos, t_vertex(0.5f, 0.5f, 1.0f));
		game_entities->push_back(staff_emitter);

	}
}

void PlayerEntity::update(float time_delta)
{
	if (apply_friction && keydown_map[LEFT] == false && keydown_map[RIGHT] == false)
	{
		if (velocity.x > 0)
		{
			velocity.x -= (0.0005);
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if (velocity.x < 0)
		{
			velocity.x += (0.0005);
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
	}
	apply_friction = false;

	position.x += real_velocity.x;
	position.y += real_velocity.y;

}

void PlayerEntity::correct_against_collisiongroup(t_collisiongroup collision_group, float time_delta)
{
	real_velocity = t_vertex(velocity.x*time_delta, velocity.y*time_delta, 0.0f);

	bool intersected = false;

	int i, j;

	for (i = 0; i < collision_group.collision_groups.size(); i++)
	{
		PolygonCollisionResult r = LinearAlgebra::PolygonCollision(return_polygon(), collision_group.collision_groups.at(i), real_velocity);

		if (r.WillIntersect) {
			// Move the polygon by its velocity, then move
			// the polygons appart using the Minimum Translation Vector

			position.x += r.MinimumTranslationVector.x;
			position.y += r.MinimumTranslationVector.y;

			if (r.MinimumTranslationVector.y > 0)
			{
				velocity.y = 0;
				apply_friction = true;
			}

			if (r.MinimumTranslationVector.x != 0 && r.MinimumTranslationVector.y == 0)
			{
				velocity.x = 0;
				// cat mode?
				velocity.y += 0.005;
			}

			if (r.MinimumTranslationVector.y < 0)
				velocity.y = -0.001;

			intersected = true;
		}
	}


}

void PlayerEntity::state_machine()
{
	if (state == IDLE && (keydown_map[LEFT] || keydown_map[RIGHT]))
	{
		if (keydown_map[LEFT])
			state = WALK_LEFT;

		if (keydown_map[RIGHT])
			state = WALK_RIGHT;
	}
	if ((state == WALK_LEFT || state == WALK_RIGHT) && (keydown_map[LEFT] == false && keydown_map[RIGHT] == false))
	{
		if(state != DEAD)
			state = IDLE;
	}

	if(state == WALK_LEFT && keydown_map[LEFT] == false)
	{ 
		state = IDLE;
	}

	if (state == WALK_RIGHT && keydown_map[RIGHT] == false)
	{
		state = IDLE;
	}

}

void PlayerEntity::player_update(float time_delta)
{
	state_machine();

	spine_data.update_skeleton(time_delta);

	if (state == WALK_LEFT || state == WALK_RIGHT)
	{
		spine_data.animation_name = "walk_two";

		if (state == WALK_LEFT)
		{
			velocity.x -= (0.00003)*time_delta;
			spine_data.flip = false;
		}

		if (velocity.x < -0.01)
			velocity.x = -0.01;

		if (state == WALK_RIGHT)
		{
			velocity.x += (0.00003)*time_delta;
			spine_data.flip = true;
		}

		if (velocity.x > 0.01)
			velocity.x = 0.01;
	}
	else if (state == CASTING)
	{
		spine_data.animation_name = "cast";
		// update emitter position;
		t_vertex emit_pos = spine_data.get_slot_location("staff_tip");
		emit_pos.x += position.x;
		emit_pos.y += position.y;
		staff_emitter->update_position(emit_pos);

		if (SDL_GetTicks() - spine_data.start_time > 1900)
		{
			state = IDLE;
			staff_emitter->kill();
		}
	}
	else if (state == DEAD)
	{
		spine_data.looping = false;
		spine_data.animation_name = "die";
	}
	else
		spine_data.animation_name = "idle";

	// jump
	if (keydown_map[ACTION] == true && state != CASTING && state != DEAD)
	{
		if (velocity.y == 0)
			velocity.y = +0.04;
	}

	if (velocity.y > -0.03)
	{
		velocity.y -= 0.0001*time_delta;
	}

}