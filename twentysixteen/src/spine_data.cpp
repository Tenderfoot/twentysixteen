
#include "spine_data.h"

/* These three functions are unused but required for using spine-c */

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
	self->rendererObject = 0;
	self->width = 123;
	self->height = 456;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self) {
}

char* _spUtil_readFile(const char* path, int* length) {
	return _readFile(path, length);
}

/* Class methods */

void SpineData::load_spine_data(char* spine_folder)
{
	atlas = spAtlas_createFromFile("data/spinedata/spineboy/spineboy.atlas", NULL);
	spSkeletonJson* skeletonJson = spSkeletonJson_create(atlas);
}