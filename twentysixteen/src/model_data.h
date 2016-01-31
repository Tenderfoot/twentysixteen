#pragma once

#pragma once

/* Class ModelData */
// Loads and stores information about 3d model files.
// paintbrush will have a map_db of all loaded models

#include <vector>
#include <SDL_opengl.h>

// Assimp, the 3d model importer...
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class t_vertex
{
public:
	t_vertex(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	t_vertex()
	{
	}

	float texcoord_x, texcoord_y;
	float x, y, z;
	float r, g, b;
};

typedef struct
{
	std::vector<t_vertex> verticies;
	t_vertex normal;
	int material_index;
} t_face;

typedef struct
{
	std::vector<t_face*> faces;
} t_mesh;

typedef struct
{
	std::vector<t_mesh*> meshes;
	std::vector<GLuint> textures;
	char *filename;
}t_3dModel;

class ModelData
{
public:
	static Assimp::Importer importer;
	static t_3dModel *import(char *filename, float scale);

	// local to the model
	GLenum shader;
	t_3dModel *model;
};

