
#include "spine_data.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SOIL\SOIL.h>

#include "paintbrush.h"

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

// by default, if I try to use the region UVs given to me from spine,
// I don't get what i'm looking for. This is me manually fixing that -
// I think it has to do with allowing for rotation in the atlas
void SpineData::setslots()
{
	int i;
	for (i = 0; i < skeleton->slotsCount; i++)
	{
		if (skeleton->slots[i] != NULL)
		{
			if (skeleton->slots[i]->attachment != NULL)
			{
				spAtlasRegion *test = spAtlas_findRegion(atlas, skeleton->slots[i]->attachment->name);
	
				spRegionAttachment* attch = (spRegionAttachment*)skeleton->slots[i]->attachment;
				float width1 = float(test->x) / 1024;
				float height1 = (float(test->y)) / 1024;
				float width2 = (float(test->x) + float(test->width)) / 1024;
				float height2 = (float(test->y) + float(test->height)) / 1024;
				spRegionAttachment_setUVs(attch, width1, height1, width2, height2, 0);
			}
		}
	}
}

void SpineData::load_spine_data(char* spine_folder)
{
	char *dir = new char[64];

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/%s.atlas", spine_folder, spine_folder);

	atlas = spAtlas_createFromFile(dir, NULL);
	spSkeletonJson* skeletonJson = spSkeletonJson_create(atlas);

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/%s.json", spine_folder, spine_folder);

	skeletonData = spSkeletonJson_readSkeletonDataFile(skeletonJson, dir);
	skeleton = spSkeleton_create(skeletonData);
	if (!skeletonData) printf("Error: %s\n", skeletonJson->error);
	spSkeletonJson_dispose(skeletonJson);

	spSkeleton_setToSetupPose(skeleton);
	spSkeleton_updateWorldTransform(skeleton);

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/%s.png", spine_folder, spine_folder);

	texture = Paintbrush::get_texture(dir, false, false);

	setslots();

	animation_name = "idle";

	delete dir;
}

void SpineData::draw()
{
	int i;
	spRegionAttachment *temp;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	for (i = 0; i < skeleton->slotsCount; i++)
	{
		temp = ((spRegionAttachment*)skeleton->slots[i]->attachment);

		if (temp != NULL)
		{
			glPushMatrix();

			//glTranslatef(0.0f, -500.0f, -950.0f);

			//glNormal3d(0, 0, 1);

			glBindTexture(GL_TEXTURE_2D, texture);
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(skeleton->slots[i]->bone->worldX, skeleton->slots[i]->bone->worldY, 0.0f);
			glRotatef(skeleton->slots[i]->bone->worldRotation, 0.0f, 0.0f, 1.0f);

			glScalef(skeleton->slots[i]->bone->worldScaleX, skeleton->slots[i]->bone->worldScaleY, 1.0f);

			glBegin(GL_QUADS);
				glTexCoord2f(temp->uvs[0], temp->uvs[1]); glVertex3f(temp->offset[0], temp->offset[1], 0.0f);
				glTexCoord2f(temp->uvs[2], temp->uvs[3]); glVertex3f(temp->offset[2], temp->offset[3], 0.0f);
				glTexCoord2f(temp->uvs[4], temp->uvs[5]); glVertex3f(temp->offset[4], temp->offset[5], 0.0f);
				glTexCoord2f(temp->uvs[6], temp->uvs[7]); glVertex3f(temp->offset[6], temp->offset[7], 0.0f);
			glEnd();

			glPopMatrix();
		}
	}
}

void SpineData::update_skeleton(float delta_time)
{
	spAnimation_apply(spSkeletonData_findAnimation(skeletonData, animation_name), skeleton, current_time/1000, (current_time+delta_time)/1000, true, NULL, NULL);
	spSkeleton_updateWorldTransform(skeleton);

	// this only probably needs to happen when animation changes
	setslots();

	current_time = SDL_GetTicks();
}