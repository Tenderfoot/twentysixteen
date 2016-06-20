#pragma once

#pragma once

/* Class LevelData */
// Loads and stores information about level's 3d model file.
// parses out important information about game entities that won't
// be drawn with the level itself, but will rather be added as entities after

#include <vector>
#include <SDL_opengl.h>

// Assimp, the 3d model importer...
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "model_data.h"

class LevelData : public ModelData
{
public:
	static t_3dModel *import(char *filename, float scale);
};

