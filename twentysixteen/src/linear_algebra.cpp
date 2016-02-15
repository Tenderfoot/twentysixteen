
#include "linear_algebra.h"
#include "paintbrush.h"

#include <math.h>

 std::vector<t_edge> *LinearAlgebra::get_edges_from_slice(t_3dModel from_model, float plane_z, t_vertex model_transform, int mesh_id)
{
	std::vector<t_edge> *edge_set = new std::vector<t_edge>;

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
			edge_set->push_back(new_edge);
		}
		//printf("hit %d times\n", hit_times);
	}

	return edge_set;
}

 bool LinearAlgebra::point_in_polygon(t_vertex point, std::vector<t_edge> edge_set)
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

	 for (auto it = edge_set.begin(); it != edge_set.end(); ++it)
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

 t_vertex LinearAlgebra::line_segment_cross_polygon(t_vertex start_point, t_vertex end_point, std::vector<t_edge> edge_set)
 {
	 // check to see if there is a line segment / polygon collision

	 // so what you're going to do is get the slope of line A
	 // take the x coordinate of line B, and use it to find x/y collision point
	 // then see if that point lies in the y range of line A

	 t_vertex collision_point(0,0,0);
	 
	 // these are verticies of an edge not edges....
	 t_vertex edge_one = edge_set.at(0).verticies.at(0);
	 t_vertex edge_two = edge_set.at(0).verticies.at(1);
	 bool hit = false;

	 int i;
	 for (i = 0; i < edge_set.size(); i++)
	 {
		 if (!hit)
		 {
			 edge_one = edge_set.at(i).verticies.at(0);
			 edge_two = edge_set.at(i).verticies.at(1);

			 float slope_one = (edge_two.y - edge_one.y) / (edge_two.x - edge_one.x);
			 float b_one = edge_two.y - (slope_one*edge_two.x);

			 float slope_two = (end_point.y - start_point.y) / (end_point.x - start_point.x);
			 float b_two = end_point.y - (slope_two*end_point.x);

			 collision_point.x = (b_two - b_one) / (slope_one - slope_two);
			 collision_point.y = slope_one*collision_point.x + b_one;

			 float small_x = std::min(start_point.x, end_point.x);
			 float big_x = std::max(start_point.x, end_point.x);
			 float small_y = std::min(start_point.y, end_point.y);
			 float big_y = std::max(start_point.y, end_point.y);

			 if (collision_point.y > small_y && collision_point.y < big_y &&
				 collision_point.x > small_x && collision_point.x < big_x)
			 {
				 hit = true;
			 }
		 }
	 }

	 if (!hit)
	 {
		 collision_point.x = 0;
		 collision_point.y = 0;
	 }

	 return collision_point;
 }


 t_vertex LinearAlgebra::get_collision_correction(t_vertex start_point, t_vertex end_point, t_collisiongroup group)
 {
	 t_vertex correction(0,0,0);

	 for (auto it = group.collision_groups.begin(); it != group.collision_groups.end(); ++it)
	 {
		 if (point_in_polygon(end_point, *it))
		 {
			 correction = line_segment_cross_polygon(start_point, end_point, *it);
			 correction.z = -1;
		 }
	 }
	
	 return correction;
 }

t_collisiongroup LinearAlgebra::get_collisiongroups_from_model(t_3dModel from_model, float plane_z, t_vertex model_transform)
 {
	 t_collisiongroup return_groups;
	 
	 int i;
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