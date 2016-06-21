
#include "vfxgrass.h"

std::vector<Entity*> VFXGrass::generate_grass(t_3dModel from_model, t_vertex model_transform, int mesh_id)
{
	std::vector<Entity*> grass_polygons;

	t_vertex new_vertex;
	GrassEntity *new_entity;

	t_collisiongroup grass_group;

	float z;

	for (z = 10; z > -50; z -= 1)
	{
		grass_group = LinearAlgebra::get_collisiongroups_from_model(from_model, z, model_transform);

		int i, j;
		for (i = 0; i < grass_group.collision_groups.size(); i++)
		{
			for (j = 0; j < grass_group.collision_groups.at(i).edges.size(); j++)
			{
				t_vertex v0 = grass_group.collision_groups.at(i).edges.at(j).verticies.at(0);
				t_vertex v1 = grass_group.collision_groups.at(i).edges.at(j).verticies.at(1);

				new_entity = new GrassEntity;

				new_entity->v1 = v0;
				new_entity->v2 = v1;
				new_entity->position.z = z;

				if (grass_group.collision_groups.at(i).edges.at(j).material_id == 2)
				{
					grass_polygons.push_back(new_entity);
				}
			}
		}
	}


	return grass_polygons;
}