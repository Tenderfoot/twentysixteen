#pragma once

/* Class Linear_Algebra */
//
// this is currently a space for me to
// create a 2D polygon out of the plane
// intersection of a 3d model for
// use with my future collision system.


#include "common.h"
#include "model_data.h"

typedef struct
{
	std::vector<t_vertex> verticies;
} t_edge;

class LinearAlgebra
{
public:

	static std::vector<t_edge> *get_edges(t_3dModel from_model);

};
