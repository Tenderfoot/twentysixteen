
#include "marshtechdemo.h"

void MarshTechDemo::init()
{
	level_static.model = ModelData::import("marsh.fbx", 0.005);
	collision_group = LinearAlgebra::get_collisiongroups_from_model(*level_static.model, 0, t_vertex(0, 0, 0));

	// Point the editor to the entity list
	level_editor.entities = &entities;
	level_editor.render_targets = &render_targets;
	level_editor.read_level("marsh");

	build_render_targets();
}
