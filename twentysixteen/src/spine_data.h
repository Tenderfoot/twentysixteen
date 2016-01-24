#pragma once

/* Class SpineData */
// Loads and stores information about spine files.
// characters and other entities will have
// their own instance of spinedata.

#include <spine/spine.h>
#include <spine/extension.h>

class SpineData
{
public:
	
	void load_spine_data(char* spine_folder);

	spSkeleton* skeleton;
	spAtlas* atlas;
	spSkeletonData *skeletonData;

};

