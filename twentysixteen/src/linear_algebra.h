#pragma once

/* Class Linear_Algebra */
// Handle linear algebra things!

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

	t_vertex as_vertex()
	{
		return t_vertex(verticies.at(1).x - verticies.at(0).x, verticies.at(1).y - verticies.at(0).y, 0);
	}

} t_edge;

typedef struct
{
	std::vector<t_edge> edges;

	t_vertex center()
	{
		int i;
		float total_x=0;
		
		float total_y=0;
		for (i = 0; i < edges.size(); i++)
		{
			total_x += edges.at(i).verticies.at(0).x;
			total_y += edges.at(i).verticies.at(0).y;
		}

		return t_vertex(total_x / edges.size(), total_y / edges.size(), 0);
	}

}t_polygon;

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
	std::vector<t_polygon> collision_groups;
} t_collisiongroup;

// collision result
typedef struct {
	// Are the polygons going to intersect forward in time?
	bool WillIntersect;
	// Are the polygons currently intersecting?
	bool Intersect;
	// The translation to apply to the first polygon to push the polygons apart.
	t_vertex MinimumTranslationVector;
} PolygonCollisionResult;

class LinearAlgebra
{
public:

	static t_polygon *get_edges_from_slice(t_3dModel from_model, float plane_z, t_vertex model_transform, int mesh_id);
	static bool is_edge_in_groups(t_edge edge, t_polygon group);
	static bool close_by(t_vertex a, t_vertex b);
	static t_collisiongroup get_collisiongroups_from_model(t_3dModel from_model, float plane_z, t_vertex model_transform);

	static bool LinearAlgebra::point_in_polygon(t_vertex point, t_polygon edge_set);
	
	// this returns whether or not a point is in a collision group:
	static bool point_in_collisiongroup(t_vertex point, t_collisiongroup group);
	
	// stuff for seperating axis theorem
	static t_vertex ProjectPolygon(t_vertex axis, t_polygon polygon);
	static float IntervalDistance(float minA, float maxA, float minB, float maxB);
	static PolygonCollisionResult PolygonCollision(t_polygon polygonA, t_polygon polygonB, t_vertex velocity);
};
