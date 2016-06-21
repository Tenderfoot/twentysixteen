#pragma once

#include "model_data.h"
#include "linear_algebra.h"
#include "entity.h"

class render_target
{
public:
	rendertarget_types type;
	t_vertex position;
	Entity *the_entity;
	t_face face;
	GLuint texture;
};