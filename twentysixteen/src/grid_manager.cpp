
#include "grid_manager.h"

// 1.5? I guess
float heuristic_cost_estimate(t_tile *a, t_tile *b)
{
	return (abs(a->x - b->x) + abs(a->y - b->y));
}


bool are_equal(t_tile *a, t_tile *b)
{
	return ((a->x == b->x) && (a->y == b->y));
}

bool in_set(std::vector<t_tile*> set, t_tile *vertex)
{
	int i;
	for (i = 0; i < set.size(); i++)
	{
		if (are_equal(set.at(i), vertex))
			return true;
	}
	return false;
}

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
			tile_map[i][j].x = i;
			tile_map[i][j].y = j;
			tile_map[i][j].gscore = INFINITY;
			tile_map[i][j].fscore = INFINITY;

		}

	tile_map[5][3].wall = 1;
	tile_map[4][3].wall = 1;
	tile_map[3][3].wall = 1;
	tile_map[2][3].wall = 1;
	tile_map[5][2].wall = 1;
	tile_map[5][1].wall = 1;
	tile_map[5][0].wall = 1;
	tile_map[7][7].wall = 1;

	tile_map[12][12].wall = 1;
	tile_map[15][15].wall = 1;
	tile_map[16][16].wall = 1;
	tile_map[17][17].wall = 1;
	tile_map[26][26].wall = 1;
	tile_map[30][30].wall = 1;
	tile_map[31][31].wall = 1;


	compute_visibility_raycast(0, 0);

	x = 0;
	y = 0;

	tile = ModelData::import("data/models/tile.fbx", 0.05);
	wall = ModelData::import("data/models/tile_wall.fbx", 0.05);

	last_path = &tile_map[0][0];
}

void GridManager::set_mouse_coords(int mx, int my)
{
	mouse_x = mx;
	mouse_y = my;

	if (mouse_x < 0)
		mouse_x = 0;
	if (mouse_x > width)
		mouse_x = width;

	if (mouse_y < 0)
		mouse_y = 0;
	if (mouse_y > height)
		mouse_y = height;

	if (!are_equal(&tile_map[mouse_x][mouse_y], last_path))
	{
		last_path = &tile_map[mouse_x][mouse_y];
		find_path(&tile_map[0][0], last_path);
	}
	
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

void GridManager::clear_path()
{
	int i2, j2;
	for(i2=0; i2<width; i2++)
		for (j2 = 0; j2 < height; j2++)
		{
			tile_map[i2][j2].gscore = INFINITY;
			tile_map[i2][j2].fscore = INFINITY;
			tile_map[i2][j2].cameFrom.x = 0;
			tile_map[i2][j2].cameFrom.y = 0;
			tile_map[i2][j2].in_path = false;
		}
}

bool GridManager::find_path(t_tile *start, t_tile *goal)
{
	clear_path();

	if (are_equal(start, goal))
		return true;

	// The set of nodes already evaluated.
	std::vector<t_tile*> closedSet = {};
		// The set of currently discovered nodes still to be evaluated.
		// Initially, only the start node is known.
	std::vector<t_tile*> openSet = { start };
		// For each node, which node it can most efficiently be reached from.
		// If a node can be reached from many nodes, cameFrom will eventually contain the
		// most efficient previous step.
	
	int i;
	int j;

	t_tile *current = start;
	t_tile *neighbour;

	current->gscore = 0;
	current->fscore = heuristic_cost_estimate(start, goal);

	while (openSet.size() > 0)
	{
		float current_fscore = INFINITY;
		for (i = 0; i < openSet.size(); i++)
			if (openSet.at(i)->fscore < current_fscore)
				current = openSet.at(i);

		if (are_equal(current, goal))
		{
			// success
			while (current != start)
			{
				current->in_path = true;
				current = &tile_map[current->cameFrom.x][current->cameFrom.y];
			}
			return true;
		}

		for (i = 0; i < openSet.size(); i++)
		{
			if (are_equal(current, openSet.at(i)))
			{
				openSet.erase(openSet.begin() + i);
			}
		}

		closedSet.push_back(current);

		for (j = 0; j < 8; j++)
		{
			int new_x, new_y;
			switch (j)
			{
			case 0:
				new_x = current->x - 1;
				new_y = current->y - 1;
				break;
			case 1:
				new_x = current->x;
				new_y = current->y - 1;
				break;
			case 2:
				new_x = current->x + 1;
				new_y = current->y - 1;
				break;
			case 3:
				new_x = current->x - 1;
				new_y = current->y;
				break;
			case 4:
				new_x = current->x + 1;
				new_y = current->y;
				break;
			case 5:
				new_x = current->x - 1;
				new_y = current->y + 1;
				break;
			case 6:
				new_x = current->x;
				new_y = current->y + 1;
				break;
			case 7:
				new_x = current->x + 1;
				new_y = current->y + 1;
				break;
			}

			if ((new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) && tile_map[new_x][new_y].wall == 0)
			{
				neighbour = &tile_map[new_x][new_y];
			}
			else
				continue;

			if (in_set(closedSet, neighbour))
				continue;		// Ignore the neighbor which is already evaluated. 

			float tentative_gScore;
			tentative_gScore = current->gscore + 1;

			if (!in_set(openSet, neighbour))	// Discover a new node
				openSet.push_back(neighbour);
			else if (tentative_gScore >= neighbour->gscore)
				continue;		// This is not a better path.

			// This path is the best until now. Record it!
			neighbour->cameFrom.x = current->x;
			neighbour->cameFrom.y = current->y;
			neighbour->gscore = tentative_gScore;
			neighbour->fscore = neighbour->gscore + heuristic_cost_estimate(neighbour, goal);
		}
	}

	return false;
}

void GridManager::draw_3d()
{
	int i2, j2;

	for (i2 = 0; i2 < width; i2++)
	{
		for (j2 = 0; j2 < height; j2++)
		{

			if (tile_map[i2][j2].visible)
				glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(0.5f, 0.5f, 0.5f);

			if (mouse_x == i2 && mouse_y == j2 && !lookmode)
				glColor3f(0.0f, 1.0f, 0.0f);

			if (tile_map[i2][j2].in_path)
				glColor3f(1.0f, 1.0f, 0.0f);

			if (tile_map[i2][j2].wall == 0)
			{
				glPushMatrix();
					glTranslatef(i2 * 5, 0.0f, j2*5);
					Paintbrush::draw_model(tile);
				glPopMatrix();
			}
			else
			{
				glPushMatrix();
					glTranslatef(i2 * 5, 0.0f, j2 * 5);
					glColor3f(1.0f, 0.5f, 0.5f);
					Paintbrush::draw_model(wall);
				glPopMatrix();
			}
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
			tile_map[i2][j2].visible = point_can_be_seen(i,j,i2,j2);
		}
	}

}

bool GridManager::point_can_be_seen(int i, int j, int i2, int j2)
{
	t_raycast vision_cast;
	vision_cast.init(i, j, i2, j2);

	// while raycasting
	while (vision_cast.has_next())
	{
		int ray_x = vision_cast.get_point().x;
		int ray_y = vision_cast.get_point().y;

		if (tile_map[ray_x][ray_y].wall == 1)
			return false;

		vision_cast.next();
	}

	return true;
}

t_polygon GridManager::get_vision_rect(int i, int j, int i2, int j2)
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