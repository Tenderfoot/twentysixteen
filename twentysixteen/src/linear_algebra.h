#pragma once

/* Class Linear_Algebra */
//
// this is currently a space for me to
// create a 2D polygon out of the plane
// intersection of a 3d model for
// use with my future collision system.


#include "common.h"
#include "model_data.h"

struct by_height {
	bool operator()(t_vertex left, t_vertex right) {
		return left.y < right.y;
	}
};

typedef struct
{
	std::vector<t_vertex> verticies;
	
	// knowing what material the face it comes from is
	// is going to help with VFXGrass
	int material_id;
} t_edge;

// edge sets are cool and all, but....
// its probably better for long-term design if
// when a model is sliced, make the edge set
// for each mesh, and then combine them into
// a "collisiongroup" structure that represents
// a group of edgesets that can be tested against.
// Then, you can call point in polygon on the COLLISIONGROUP,
// and it will check each edge set.
//
// This makes it okay if there are overlaps in meshes, which would usually
// ruin the point_in_polygon test.

typedef struct
{
	// a vector, that contains edge_sets, which are vectors of t_edge.
	std::vector<std::vector<t_edge>> collision_groups;
} t_collisiongroup;

class LinearAlgebra
{
public:

	static std::vector<t_edge> *get_edges_from_slice(t_3dModel from_model, float plane_z, t_vertex model_transform, int mesh_id);
	static t_collisiongroup get_collisiongroups_from_model(t_3dModel from_model, float plane_z, t_vertex model_transform);

	static bool point_in_polygon(t_vertex point, std::vector<t_edge> edge_set);
	// this returns whether or not a point is in a collision group:
	static bool point_in_collisiongroup(t_vertex point, t_collisiongroup group);
	// this checks the line segment created from the start and end points, against the collision group. If intersections are found, it takes the closest,
	// and returns the vector of translation required to remove the vertex from the collisiongroup
	static t_vertex get_collision_correction(t_vertex start_point, t_vertex end_point, t_collisiongroup group);
	static t_vertex line_segment_cross_polygon(t_vertex start_point, t_vertex end_point, std::vector<t_edge> edge_set);

};
