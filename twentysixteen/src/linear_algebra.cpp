
#include "linear_algebra.h"

 std::vector<t_edge> *LinearAlgebra::get_edges(t_3dModel from_model)
{
	std::vector<t_edge> *edge_set = new std::vector<t_edge>;

	t_edge new_edge;

	int i, j;

	int mesh_id = 0;

	float plane_distance = (-20);
	float model_z_translation = -50;
	int hit_times = 0;
	
	for (i = 0; i < from_model.meshes.at(mesh_id)->faces.size(); i++)
	{
		hit_times = 0;
		for (j = 0; j < 3; j++)
		{
			if ((from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).z + model_z_translation < plane_distance && from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).z + model_z_translation > plane_distance) ||
				(from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).z + model_z_translation < plane_distance && from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).z + model_z_translation > plane_distance))
			{
				hit_times++;

				new_edge.v1.x = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).x;
				new_edge.v1.y = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at(j).y;

				new_edge.v2.x = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).x;
				new_edge.v2.y = from_model.meshes.at(mesh_id)->faces.at(i)->verticies.at((j + 1) % 3).y;

				edge_set->push_back(new_edge);
			}
		}
		printf("hit %d times\n", hit_times);
	}

	return edge_set;
}