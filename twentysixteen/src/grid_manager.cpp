
#include "grid_manager.h"

void GridManager::init(int w, int h)
{
	width = w;
	height = h;

	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
		{
			tile_map[i][j] = t_tile();
			tile_map[i][j].wall = 0;
		}

	tile_map[5][3].wall = 1;
	tile_map[4][3].wall = 1;
	tile_map[3][3].wall = 1;
	tile_map[2][3].wall = 1;
	tile_map[5][2].wall = 1;
	tile_map[5][1].wall = 1;
	tile_map[5][0].wall = 1;
	tile_map[7][7].wall = 1;

	compute_visibility_raycast(0, 0);

	x = 0;
	y = 0;
}

void GridManager::draw_2d()
{
	int p;
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
		{
			if (tile_map[i][j].visible == true || tile_map[i][j].wall == 1)
			{
				glPushMatrix();
				glTranslatef(i, j, 0.0f);
				glDisable(GL_TEXTURE_2D);

				if (tile_map[i][j].wall == 1)
					glColor3f(1.0f, 0.0f, 0.0f);
				else if (i == x && j == y)
					glColor3f(1.0f, 1.0f, 0.0f);
				else
					glColor3f(((float)i) / width, ((float)j) / height, 1.0f);
				Paintbrush::draw_quad();
				glPopMatrix();
				glEnable(GL_TEXTURE_2D);
			}
		}
}

void GridManager::compute_visibility(int i, int j)
{
	t_polygon vision_rect;
	bool found;
	int i2, j2;

	for (i2 = 0; i2 < width; i2++)
	{
		for (j2 = 0; j2 < height; j2++)
		{
			tile_map[i2][j2].visible = true;
			vision_rect = get_vision_rect(i, j, i2, j2);
			int a, b;
			for (a = 0; a < width; a++)
				for (b = 0; b < height; b++)
				{
					if (check_collision(vision_rect, a, b) == true)
					{
						if (tile_map[a][b].wall == 1)
						{
							tile_map[i2][j2].visible = false;
						}
					}
				}
		}
	}
}

void GridManager::compute_visibility_raycast(int i, int j)
{
	t_polygon vision_rect;
	bool found;
	int i2, j2;

	// i and j are the current position
	// i2 and j2 are iterators.
	// for the current position cast a ray from the current position
	// to a position on the perimeter. 

	for (i2 = 0; i2 < width; i2++)
	{
		for (j2 = 0; j2 < height; j2++)
		{
			tile_map[i2][j2].visible = false;
		}
	}

	t_raycast vision_cast;

	for (i2 = 0; i2 < width; i2++)
	{
		for (j2 = 0; j2 < height; j2++)
		{
			vision_cast.init(i, j, i2, j2);

			// while raycasting
			while (vision_cast.has_next())
			{
				int ray_x = vision_cast.get_point().x;
				int ray_y = vision_cast.get_point().y;

				if (tile_map[ray_x][ray_y].wall == 1)
					break;

				tile_map[ray_x][ray_y].visible = true;
				vision_cast.next();
			}
		}
	}

}

t_polygon GridManager::get_vision_rect(int i, int j, float i2, float j2)
{
	t_polygon return_rect;
	float slope;

	if (i2 - i != 0)
	{
		slope = ((((float)j2) - (float)j) / (((float)i2) - (float)i));
	}
	else
	{
		slope = INFINITY;
	}

	float inv_slope;
	if (slope == 0)
	{
		inv_slope == INFINITY;
	}
	else
		inv_slope = -1 / slope;

	float length = sqrt(1 + ((inv_slope)*(inv_slope)));

	inv_slope = inv_slope / length;

	t_vertex v1, v2, v3, v4;

	float box_scale = 0.001;

	v1.x = i2 + box_scale;
	v2.x = i2 - box_scale;
	v1.y = j2 + box_scale*(inv_slope);
	v2.y = j2 - box_scale*(inv_slope);

	v3.x = i + box_scale;
	v4.x = i - box_scale;
	v3.y = j + box_scale*(inv_slope);
	v4.y = j - box_scale*(inv_slope);

	t_edge new_edge;
	new_edge.verticies.push_back(v1);
	new_edge.verticies.push_back(v2);
	return_rect.edges.push_back(new_edge);
	new_edge.verticies.clear();

	new_edge.verticies.push_back(v2);
	new_edge.verticies.push_back(v4);
	return_rect.edges.push_back(new_edge);
	new_edge.verticies.clear();

	new_edge.verticies.push_back(v4);
	new_edge.verticies.push_back(v3);
	return_rect.edges.push_back(new_edge);
	new_edge.verticies.clear();

	new_edge.verticies.push_back(v3);
	new_edge.verticies.push_back(v1);
	return_rect.edges.push_back(new_edge);
	new_edge.verticies.clear();

	return return_rect;
}

bool GridManager::check_collision(t_polygon rect, int i, int j)
{
	GameEntity test_against;
	test_against.size = t_vertex(1, 1, 0);
	test_against.position = t_vertex(i, j, 0);

	return LinearAlgebra::PolygonCollision(rect, test_against.return_polygon(), t_vertex(0, 0, 0)).Intersect;
}