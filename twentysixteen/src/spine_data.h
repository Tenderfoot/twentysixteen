#pragma once

/* Class SpineData */
// Loads and stores information about spine files.
// characters and other entities will have
// their own instance of spinedata.

#include <SDL_opengl.h>

#include <spine/spine.h>
#include <spine/extension.h>

class SpineData
{
public:
	
	void load_spine_data(char* spine_folder);
	void setslots();
	void update_skeleton(float delta_time);

	char *animation_name;
	float current_time;

	spSkeleton* skeleton;
	spAtlas* atlas;
	spSkeletonData *skeletonData;
	bool flip;

	GLuint texture; 

	void draw();
};

