#pragma once

/* Class Linear_Algebra */
//
// this is currently a space for me to
// create a 2D polygon out of the plane
// intersection of a 3d model for
// use with my future collision system.


#include "common.h"
#include "model_data.h"

struct by_depth {
	bool operator()(t_vertex left, t_vertex right) {
		return left.y < right.y;
	}
};

typedef struct
{
	std::vector<t_vertex> verticies;
} t_edge;

class LinearAlgebra
{
public:

	static std::vector<t_edge> *get_edges_from_plane(t_3dModel from_model);
	static bool point_in_polygon(t_vertex point, std::vector<t_edge> edge_set);

};
