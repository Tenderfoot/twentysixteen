#pragma once

#include "common.h"
#include "level.h"
#include "render_target.h"
#include "vfxgrass.h"
#include "leveleditor.h"
#include "particles.h"

// BaseGameLevel
// This is the base for all the scenes in the game.
// Level is used for everything, menus, etc...
// This abstraction allows for more of the game specific
// code to be properly organized. Each Scene in the game
// will inherit from this, and sooner than that it will be
// used to prototype the different scenes.

class BaseGameLevel : public Level
{
public:

	void build_render_targets();
	void set_camera(t_vertex position, t_vertex lookat);
	void run(float time_delta);
	void draw();

	// Model for area
	ModelData level_static;
	t_collisiongroup collision_group;

	// Level Editor
	LevelEditor level_editor;

	// camera
	t_vertex camera_position, camera_lookat;

	// Level Entities
	std::vector<Entity*> entities;

	// Render target vector for sorted rendering
	std::vector<render_target> render_targets;

	struct by_depth_rendertarget {
		bool operator()(render_target left, render_target right) {
			return left.position.z < right.position.z;
		}
	};
};