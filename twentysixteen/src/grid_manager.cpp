#include <sstream>
#include "grid_manager.h"

// 1.5? I guess
float heuristic_cost_estimate(t_tile *a, t_tile *b)
{
	return (abs(b->x - a->x) + abs(b->y - a->y));
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


int GridManager::entity_on_position(t_vertex entity_pos)
{
	int i;
	for (i = 0; i < entities->size(); i++)
	{
		if (entity_pos.x == entities->at(i)->position.x && entity_pos.z == entities->at(i)->position.z && entities->at(i)->type != GRID_SPAWNPOINT && entities->at(i)->type != GRID_ENEMYSPAWNPOINT)
		{
			return i;
		}
	}
	return -1;
}

void GridManager::draw_path(t_vertex start_pos)
{
	std::vector<t_tile*> test = find_path(start_pos, t_vertex(mouse_x, 0, mouse_y));

	int i;
	if (test.size() > 0)
	{
		for (i = 1; i < test.size(); i++)
		{
			test[i]->in_path = true;
		}
	}
}

int GridManager::num_path(t_vertex start_pos)
{
	int b = 0;
	std::vector<t_tile*> test = find_path(start_pos, t_vertex(mouse_x, 0, mouse_y));

	int i;
	for (i = 0; i < test.size(); i++)
	{
		b++;
		test[i]->in_path = true;
	}
	return b;
}

bool GridManager::position_visible(int x, int z)
{
	return tile_map[x][z].visible;
}

void GridManager::load_map(std::string mapname)
{
	std::stringstream filename;
	filename << "data/levels/" << mapname.c_str() << ".txt";
	std::ifstream in(filename.str());
	std::string line;

	// get number of entities
	std::getline(in, line);
	int w = std::stoi(line);

	std::getline(in, line);
	int h = std::stoi(line);

	width = w;
	height = h;

	int i, j;

	i = 0;
	j = 0;

	Entity *grid_spawn = NULL;
	while (std::getline(in, line))
	{
		for (i = 0; i < line.length(); i++)
		{
			tile_map[i][j] = t_tile();

			int tile_type = line.c_str()[i] - '0';

			switch (tile_type)
			{
			case 0:
				tile_map[i][j].wall = 0;
				break;
			case 1:
				tile_map[i][j].wall = 1;
				break;
			case 2:
				tile_map[i][j].wall = 0;
				grid_spawn = new Entity();
				grid_spawn->position.x = i;
				grid_spawn->position.z = j;
				grid_spawn->type = GRID_SPAWNPOINT;
				entities->push_back(grid_spawn);
				break;
			case 3:
				tile_map[i][j].wall = 0;
				grid_spawn = new Entity();
				grid_spawn->position.x = i;
				grid_spawn->position.z = j;
				grid_spawn->type = GRID_ENEMYSPAWNPOINT;
				entities->push_back(grid_spawn);
				break;
			}

			tile_map[i][j].x = i;
			tile_map[i][j].y = j;
			tile_map[i][j].gscore = INFINITY;
			tile_map[i][j].fscore = INFINITY;
			tile_map[i][j].discovered = false;
		}

		j++;
	}
}

void GridManager::init()
{
	load_map("gridmap");
	
	tile = ModelData::import("data/models/tile.fbx", 0.05);
	wall = ModelData::import("data/models/tile_wall.fbx", 0.05);
	autotile_tex = Paintbrush::Soil_Load_Texture("data/images/war2autotile_grasstodirt.png", false, false);
	autotile_tex_war2 = Paintbrush::Soil_Load_Texture("data/images/war2autotile_grasstodirt_real.png", false, false);

	active_autotile = autotile_tex;

	last_path = &tile_map[x][y];
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
		find_path(t_vertex(x,0,y), t_vertex(last_path->x, 0.0f, last_path->y));
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

std::vector<t_tile*> GridManager::find_path(t_vertex start_pos, t_vertex end_pos)
{
	t_tile *start = &tile_map[start_pos.x][start_pos.z];
	t_tile *goal = &tile_map[end_pos.x][end_pos.z];
	clear_path();

	std::vector<t_tile*> return_vector;

	if (are_equal(start, goal))
		return return_vector;

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
			{
				current = openSet.at(i);
				current_fscore = current->fscore;
			}

		if (are_equal(current, goal))
		{
			// success
			while (current != start)
			{
				return_vector.push_back(current);
				// this made the path yellow
				// which is cool, but shouldn't be done here..
				//current->in_path = true;
				current = &tile_map[current->cameFrom.x][current->cameFrom.y];
			}
			return return_vector;
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
			bool valid = true;
			switch (j)
			{
			case 0:
				new_x = current->x - 1;
				new_y = current->y - 1;
				if (tile_map[current->x - 1][current->y].wall == 1 || tile_map[current->x][current->y - 1].wall == 1)
					valid = false;
				break;
			case 1:
				new_x = current->x;
				new_y = current->y - 1;
				break;
			case 2:
				new_x = current->x + 1;
				new_y = current->y - 1;
				if (tile_map[current->x + 1][current->y].wall == 1 || tile_map[current->x][current->y - 1].wall == 1)
					valid = false;
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
				if (tile_map[current->x - 1][current->y].wall == 1 || tile_map[current->x][current->y + 1].wall == 1)
					valid = false;
				break;
			case 6:
				new_x = current->x;
				new_y = current->y + 1;
				break;
			case 7:
				new_x = current->x + 1;
				new_y = current->y + 1;
				if (tile_map[current->x + 1][current->y].wall == 1 || tile_map[current->x][current->y + 1].wall == 1)
					valid = false;
				break;
			}

			if ((new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) && tile_map[new_x][new_y].wall == 0 && entity_on_position(t_vertex(new_x,0,new_y)) == -1 && valid)
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

	return return_vector;
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
			{
				if(good_spot)
					glColor3f(0.0f, 1.0f, 0.0f);
				else
					glColor3f(1.0f, 0.0f, 0.0f);
			}

			if (tile_map[i2][j2].in_path)
				glColor3f(1.0f, 1.0f, 0.0f);

			if (tile_map[i2][j2].discovered)
			{
				if (tile_map[i2][j2].wall == 0)
				{
					glPushMatrix();
					glTranslatef(i2 * 5, 0.0f, j2 * 5);
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
}

void GridManager::randomize_map()
{

	for (int i = 1; i < width - 2; i++)
	{
		for (int j = 1; j < height - 2; j++)
		{
			tile_map[i][j].wall = 0;
		}
	}

	for (int i = 1; i < width - 2; i++)
	{
		for (int j = 1; j < height - 2; j++)
		{
			if(rand() % 5 == 0)
			{ 
				tile_map[i][j].wall = 1;
				tile_map[i+1][j].wall = 1;
				tile_map[i][j+1].wall = 1;
				tile_map[i+1][j+1].wall = 1;
			}
		}
	}
	
}

// TODO:
// - Fix table
// - Rotate and randomize similar tiles
// - add water tileset

static const int war2_autotile_map[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, 13, 13, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, 13, -1, -1,
										13, 13, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, 7, 6, -1, 7,
										7, 6, 5, 5, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, 9, 9,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, 7, -1,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										7, 6, -1, -1, 7, 6, 5, 5, -1, -1,
										-1, 14, -1, -1, -1, 14, -1, -1, -1, -1,
										-1, -1, -1, 12, -1, -1, -1, 6, -1, -1,
										-1, 6, -1, -1, -1, 14, -1, -1, -1, 12,
										-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1, -1, -1, 11, -1, 14,
										-1, -1, -1, 14, -1, 7, -1, 6, -1, -1,
										-1, 4, -1, -1, -1, 14, -1, -1, 14, 14,
										-1, -1, -1, -1, -1, -1, 13, 12, -1, -1,
										-1, 6, -1, -1, -1, 6, -1, -1, -1, 14,
										-1, -1, -1, 12, 11, 11, -1, 10, 9, 9,
										11, 10, -1, -1, -1, -1, -1, 11, 9, 8,
										11, 11, -1, 10, 9, 9, -1, 10, 3, 3,
										-1, 2, 3, 3, 1, 0 };
//183=14
void GridManager::draw_autotile()
{
	int i, j;
	bool test;

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			int tex_wall = 0;
			test = false;

			if(i==0 || i==width-1 || j==0 || j==height-1)
			{
				tex_wall = 15;
			}
			else
			{
				if (tile_map[i - 1][j - 1].wall == 1)
					tex_wall = (tex_wall | 1);

				if (tile_map[i][j - 1].wall == 1)
					tex_wall = (tex_wall | 2);

				if (tile_map[i + 1][j - 1].wall == 1)
					tex_wall = (tex_wall | 4);

				if (tile_map[i + 1][j].wall == 1)
					tex_wall = (tex_wall | 8);

				if (tile_map[i + 1][j + 1].wall == 1)
					tex_wall = (tex_wall | 16);

				if (tile_map[i][j + 1].wall == 1)
					tex_wall = (tex_wall | 32);

				if (tile_map[i - 1][j + 1].wall == 1)
					tex_wall = (tex_wall | 64);

				if (tile_map[i - 1][j].wall == 1)
					tex_wall = (tex_wall | 128);


				if (war2_autotile_map[tex_wall] != -1)
				{
					tex_wall = war2_autotile_map[tex_wall];
				}
				else
					tex_wall = 15;
			}

			if (tile_map[i][j].wall == 0)
			{
				test = true;
				tex_wall = 15;
			}
			int xcoord = tex_wall % 4;
			int ycoord = tex_wall / 4;

			glPushMatrix();
				glTranslatef(i * 5, 0.0f, j * 5);
				glRotatef(90, 1.0f, 0.0f, 0.0f);
				glScalef(2.5f, 2.5f, 1.0f);
				if (test)
					glColor3f(1.0f, 0.0f, 1.0f);
				else
					glColor3f(1.0f, 1.0f, 1.0f);

				glBindTexture(GL_TEXTURE_2D, active_autotile);
				glPushMatrix();
					glBegin(GL_QUADS);
						glTexCoord2f(0.25f + (0.25f*xcoord), 0.25f + (0.25f*ycoord));	glVertex3f(1.0f, 1.0f, 0.0f);
						glTexCoord2f(0.0f + (0.25f*xcoord), 0.25f + (0.25f*ycoord));	glVertex3f(-1.0f, 1.0f, 0.0f);
						glTexCoord2f(0.0f + (0.25f*xcoord), 0.0f + (0.25f*ycoord));	glVertex3f(-1.0f, -1.0f, 0.0f);
						glTexCoord2f(0.25f + (0.25f*xcoord), 0.0f + (0.25f*ycoord));	glVertex3f(1.0f, -1.0f, 0.0f);
					glEnd();
				glPopMatrix();
			glPopMatrix();
		}
	}
}

void GridManager::reset_visibility()
{
	for (int i2 = 0; i2 < width; i2++)
	{
		for (int j2 = 0; j2 < height; j2++)
		{
			tile_map[i2][j2].visible = false;
		}
	}
}

void GridManager::compute_visibility_raycast(int i, int j, bool discover)
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
			if(!tile_map[i2][j2].visible)
				tile_map[i2][j2].visible = point_can_be_seen(i,j,i2,j2);

			if (tile_map[i2][j2].visible && discover)
				tile_map[i2][j2].discovered = true;
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

bool GridManager::check_collision(t_polygon rect, int i, int j)
{
	GameEntity test_against;
	test_against.size = t_vertex(1, 1, 0);
	test_against.position = t_vertex(i, j, 0);

	return LinearAlgebra::PolygonCollision(rect, test_against.return_polygon(), t_vertex(0, 0, 0)).Intersect;
}