
#include "linear_algebra.h"
#include "paintbrush.h"

#include <math.h>

t_polygon *LinearAlgebra::get_edges_from_slice(t_3dModel from_model, float plane_z, t_vertex model_transform, int mesh_id)
{
	t_polygon *edge_set = new t_polygon;

	t_edge new_edge;
	t_vertex new_vertex;

	int i, j;

	float plane_distance = plane_z - model_transform.z;
	int hit_times = 0;
	
	//mesh_id = 1;

	for (i = 0; i < from_model.meshes.at(mesh_id)->faces.size(); i++)
	{
		new_edge.verticies.clear();

		hit_times = 0;
		for (j = 0; j < 3; j++)
		{
			if ((from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).z < plane_distance && from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).z > plane_distance) ||
				(from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).z  < plane_distance && from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).z  > plane_distance))
			{
				hit_times++;
				
				t_vertex initial_point, other_point, swap;
				initial_point = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j);
				other_point = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3);

				if (initial_point.z > other_point.z)
				{
					swap = initial_point;
					initial_point = other_point;
					other_point = swap;
				}

				float percentage = (plane_distance - initial_point.z) / (other_point.z - initial_point.z);

				new_vertex.x = initial_point.x + ((other_point.x - initial_point.x)*percentage);
				new_vertex.y = initial_point.y + ((other_point.y - initial_point.y)*percentage);
				new_vertex.z = plane_z;

				new_edge.verticies.push_back(new_vertex);
			}
		}

		if (new_edge.verticies.size() == 2)
		{
			new_edge.material_id = from_model.meshes.at(mesh_id)->faces.at(i)->material_index;
			edge_set->edges.push_back(new_edge);
		}
	}



	return edge_set;
}

 bool LinearAlgebra::point_in_polygon(t_vertex point, t_polygon edge_set)
 {
	 // basic idea is to take the x coordinate of the point, and
	 // get all points of intersection with the edge set. Once this
	 // is done, sort the points of intersection from bottom to top -
	 // you can determine if the point is in the plane depending on
	 // whether the set of points its between is even or odd
	 // https://en.wikipedia.org/wiki/Point_in_polygon#/media/File:RecursiveEvenPolygon.svg

	 std::vector<t_vertex> matching_points;
	 std::vector<t_edge>::iterator it;
	 t_vertex new_vertex;

	 for (auto it = edge_set.edges.begin(); it != edge_set.edges.end(); ++it)
	 {
		 if ((it->verticies.at(0).x < point.x && it->verticies.at(1).x > point.x) ||
			 (it->verticies.at(1).x < point.x && it->verticies.at(0).x > point.x))
		 {
			 // Xs match, add intersection point to point set
			 t_vertex initial_point, other_point, swap;
			 initial_point = it->verticies.at(0);
			 other_point = it->verticies.at(1);

			 float percentage = (point.x + initial_point.x) / (other_point.x + initial_point.x);

			 new_vertex.x = point.x;
			 new_vertex.y = initial_point.y + ((other_point.y - initial_point.y)*percentage);
			 matching_points.push_back(new_vertex);
		 }
	 }

	 std::sort(matching_points.begin(), matching_points.end(), by_height());

	 bool hit = true;

	 // using iterator here was giving me some flak 
	 // [debug assertion on dereferencing it, I was trying to get the current item and next item]
	 // [hook me up if theres a better way to do this]
	 int i;
	 for (i=0; i < matching_points.size(); i++)
	 {
		if (point.y > matching_points.at(i).y && point.y < matching_points.at((i+1)%matching_points.size()).y)
		 {
			 return hit;
		 }
		 hit = !hit;
	 }

	 return false;
 }

 t_vertex LinearAlgebra::ProjectPolygon(t_vertex axis, t_polygon polygon) 
 {
	 // To project a point on an axis use the dot product
	 if (polygon.edges.size() > 0)
	 {
		 float dotProduct = axis.DotProduct(polygon.edges.at(0).verticies.at(0));

		 t_vertex minmax;
		 minmax.x = dotProduct;
		 minmax.y = dotProduct;
		 for (int i = 0; i < polygon.edges.size(); i++) {
			 dotProduct = polygon.edges.at(i).verticies.at(0).DotProduct(axis);
			 if (dotProduct < minmax.x) {
				 minmax.x = dotProduct;
			 }
			 else {
				 if (dotProduct > minmax.y) {
					 minmax.y = dotProduct;
				 }
			 }
		 }

		 return minmax;
	 }
	 return t_vertex(0, 0, 0);
 }

 float LinearAlgebra::IntervalDistance(float minA, float maxA, float minB, float maxB) 
 {
	 if (minA < minB) {
		 return minB - maxA;
	 }
	 else {
		 return minA - maxB;
	 }
 }

 PolygonCollisionResult LinearAlgebra::PolygonCollision(t_polygon polygonA, t_polygon polygonB, t_vertex velocity)
 {
	 PolygonCollisionResult result = PolygonCollisionResult();
	 result.Intersect = true;
	 result.WillIntersect = true;

	 int edgeCountA = polygonA.edges.size();
	 int edgeCountB = polygonB.edges.size();
	 float minIntervalDistance = 999999999;
	 t_vertex translationAxis = t_vertex();
	 t_vertex edge;

	 // Loop through all the edges of both polygons
	 for (int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++) {
		 if (edgeIndex < edgeCountA) {
			 edge = polygonA.edges.at(edgeIndex).as_vertex();
		 }
		 else {
			 edge = polygonB.edges.at(edgeIndex - edgeCountA).as_vertex();
		 }

		 // ===== 1. Find if the polygons are currently intersecting =====

		 // Find the axis perpendicular to the current edge
		 t_vertex axis = t_vertex(-edge.y, edge.x, 0);
		 axis.Normalize();
		 
		 // Find the projection of the polygon on the current axis
		 float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
		 minA = ProjectPolygon(axis, polygonA).x;
		 maxA = ProjectPolygon(axis, polygonA).y;
		 minB = ProjectPolygon(axis, polygonB).x;
		 maxB = ProjectPolygon(axis, polygonB).y;

		 // Check if the polygon projections are currentlty intersecting
		 if (IntervalDistance(minA, maxA, minB, maxB) > 0)
		 {
			 result.Intersect = false;
		 }

		 // ===== 2. Now find if the polygons *will* intersect =====

		 // Project the velocity on the current axis
		 float velocityProjection = axis.DotProduct(velocity);

		 // Get the projection of polygon A during the movement
		 if (velocityProjection < 0) {
			 minA += velocityProjection;
		 }
		 else {
			 maxA += velocityProjection;
		 }

		 // Do the same test as above for the new projection
		 float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
		 if (intervalDistance > 0) result.WillIntersect = false;

		 // If the polygons are not intersecting and won't intersect, exit the loop
		 if (!result.Intersect && !result.WillIntersect) break;

		 // Check if the current interval distance is the minimum one. If so store
		 // the interval distance and the current distance.
		 // This will be used to calculate the minimum translation vector
		 intervalDistance = abs(intervalDistance);
		 if (intervalDistance < minIntervalDistance) {
			 minIntervalDistance = intervalDistance;
			 translationAxis = axis;

			 t_vertex d = t_vertex(polygonA.center().x - polygonB.center().x, polygonA.center().y - polygonB.center().y, 0);
			 if (d.DotProduct(translationAxis) < 0)
			 {
				 translationAxis.x = -translationAxis.x;
				 translationAxis.y = -translationAxis.y;
			 }
		}
	 }

	 // The minimum translation vector
	 // can be used to push the polygons appart.
	 if (result.WillIntersect)
		 result.MinimumTranslationVector =
		 t_vertex(translationAxis.x * minIntervalDistance, translationAxis.y * minIntervalDistance,0);

	 return result;
 }

 bool LinearAlgebra::close_by(t_vertex a, t_vertex b)
 {
	 float threshhold = 0.1;

	 if (a.x > b.x - threshhold && a.x < b.x + threshhold && a.y > b.y - threshhold && a.y < b.y + threshhold)
		 return true;
	 else
		 return false;
 }

 bool LinearAlgebra::is_edge_in_groups(t_edge edge, t_polygon polygon)
 {
	 int i, j;
	 t_edge test_edge;

	 for (i = 0; i < polygon.edges.size(); i++)
	 {
		 test_edge = polygon.edges.at(i);
		 if (close_by(edge.verticies.at(0),test_edge.verticies.at(0)) ||
			 close_by(edge.verticies.at(1), test_edge.verticies.at(0)) ||
			 close_by(edge.verticies.at(1), test_edge.verticies.at(1)) ||
			 close_by(edge.verticies.at(0), test_edge.verticies.at(1)))
		 {
			 return true;
		 }
	 }
	 return false;
}

t_collisiongroup LinearAlgebra::get_collisiongroups_from_model(t_3dModel from_model, float plane_z, t_vertex model_transform)
 {
	 t_collisiongroup return_groups, split_groups;
	 
	 int i,j,k;

	 for (i = 0; i < from_model.meshes.size(); i++)
	 {
		 return_groups.collision_groups.push_back(*get_edges_from_slice(from_model, plane_z, model_transform, i));
	 }


	 return return_groups;
 }

bool  LinearAlgebra::point_in_collisiongroup(t_vertex point, t_collisiongroup group)
{
	bool return_value = false;

	for (auto it = group.collision_groups.begin(); it != group.collision_groups.end(); ++it)
	{
		if(return_value == false)
			return_value = point_in_polygon(point, *it);
	}

	return return_value;
}