
#include "spine_data.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SOIL\SOIL.h>

#include "paintbrush.h"

/* These three functions are unused but required for using spine-c */

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path) {
	self->rendererObject = 0;

	glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(std::string(path), false, false));

	int w, h;
	int miplevel = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);

	std::string new_string(path);
	self->width = w;
	self->height = h;
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

	float texwidth, texheight;

	if (spine_name == "everybody")
	{
		texwidth = 4096;
		texheight = 512;
	}
	if (spine_name == "cat")
	{
		texwidth = 1024;
		texheight = 256;
	}
	if (spine_name == "test")
	{
		texwidth = 512;
		texheight = 256;
	}
	if (spine_name == "cat2")
	{
		texwidth = 1024;
		texheight = 256;
	}
	if (spine_name == "hero")
	{
		texwidth = 512;
		texheight = 256;
	}
	if (spine_name == "heromesh")
	{
		texwidth = 1024;
		texheight = 256;
	}

	if (spine_name == "meshtest")
	{
		texwidth = 256;
		texheight = 256;
	}


	if (spine_name == "spinemesh")
	{
		texwidth = 1024;
		texheight = 1024;
	}

	for (i = 0; i < skeleton->slotsCount; i++)
	{
		if (skeleton->slots[i] != NULL)
		{
			if (skeleton->slots[i]->attachment != NULL)
			{
				if (skeleton->slots[i]->attachment->type == SP_ATTACHMENT_REGION)
				{
					spAtlasRegion *test = spAtlas_findRegion(atlas, skeleton->slots[i]->attachment->name);

					spRegionAttachment* attch = (spRegionAttachment*)skeleton->slots[i]->attachment;
					float width1 = float(test->x) / texwidth;
					float height1 = (float(test->y)) / texheight;
					float width2 = (float(test->x) + float(test->width)) / texwidth;
					float height2 = (float(test->y) + float(test->height)) / texheight;
					spRegionAttachment_setUVs(attch, width1, height1, width2, height2, 0);
				}

				if (skeleton->slots[i]->attachment->type == SP_ATTACHMENT_MESH)
				{
					spMeshAttachment_updateUVs((spMeshAttachment*)skeleton->slots[i]->attachment);
				}
			}
		}
	}
}

void SpineData::load_spine_data(char* spine_folder)
{
	char *dir = new char[64];

	spine_name = spine_folder;

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/skeleton.atlas", spine_folder);

	atlas = spAtlas_createFromFile(dir, NULL);
	spSkeletonJson* skeletonJson = spSkeletonJson_create(atlas);

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/skeleton.json", spine_folder);

	skeletonData = spSkeletonJson_readSkeletonDataFile(skeletonJson, dir);
	skeleton = spSkeleton_create(skeletonData);
	if (!skeletonData) printf("Error: %s\n", skeletonJson->error);
	spSkeletonJson_dispose(skeletonJson);

	spSkeleton_setToSetupPose(skeleton);

	spSkeleton_updateWorldTransform(skeleton);

	sprintf_s(dir, sizeof(char)*64, "data/spinedata/%s/skeleton.png", spine_folder);

	texture = Paintbrush::get_texture(dir, false, false);

	animation_name = "idle";

	setslots();

	delete dir;
}

void SpineData::draw_regionattachment(int i)
{
	spRegionAttachment *temp;
	temp = ((spRegionAttachment*)skeleton->slots[i]->attachment);

	glPushMatrix();

		glNormal3d(0, 0, 1);

		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1.0f, 1.0f, 1.0f);

		glTranslatef(skeleton->slots[i]->bone->worldX, skeleton->slots[i]->bone->worldY, 0.0f);
		glRotatef(spBone_getWorldRotationX(skeleton->slots[i]->bone), 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(temp->uvs[0], temp->uvs[1]); glVertex3f(temp->offset[0], temp->offset[1], 0.0f);
			glTexCoord2f(temp->uvs[2], temp->uvs[3]); glVertex3f(temp->offset[2], temp->offset[3], 0.0f);
			glTexCoord2f(temp->uvs[4], temp->uvs[5]); glVertex3f(temp->offset[4], temp->offset[5], 0.0f);
			glTexCoord2f(temp->uvs[6], temp->uvs[7]); glVertex3f(temp->offset[6], temp->offset[7], 0.0f);
		glEnd();

	glPopMatrix();
}

void SpineData::draw_meshattachment(int i)
{
	spMeshAttachment *temp;
	temp = ((spMeshAttachment*)skeleton->slots[i]->attachment);

	int j;

	float *world_verticies = new float[((spVertexAttachment*)temp)->verticesCount];
	spMeshAttachment_computeWorldVertices(temp, skeleton->slots[i], world_verticies);
	spMeshAttachment_updateUVs(temp);

	spAtlasRegion *test = spAtlas_findRegion(atlas, skeleton->slots[i]->attachment->name);

	glPushMatrix();

		//glTranslatef(0.0f, -500.0f, -950.0f)
		glNormal3d(0, 0, 1);

		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1.0f, 1.0f, 1.0f);

		int index;
		glBegin(GL_TRIANGLES);
		for (j = 0; j < temp->trianglesCount; j++)
		{
			index = temp->triangles[j] << 1;

			float new_u = (temp->uvs[index]);
			float new_v = (temp->uvs[index + 1]);

			glTexCoord2f(new_u, new_v); glVertex3f(world_verticies[index], world_verticies[index+1], 0.0f);
		}
		glEnd();

	glPopMatrix();

	delete world_verticies;
}

void SpineData::draw()
{
	int i;
	spRegionAttachment *temp;

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glPushMatrix();
	if (flip)
		glRotatef(180, 0, 1, 0);

	for (i = 0; i < skeleton->slotsCount; i++)
	{
		if (skeleton->slots[i]->attachment != NULL)
		{
			if (skeleton->slots[i]->attachment->type == SP_ATTACHMENT_REGION)
			{
				draw_regionattachment(i);
			}
			if (skeleton->slots[i]->attachment->type == SP_ATTACHMENT_MESH)
			{
				draw_meshattachment(i);
			}
		}
	}
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

t_vertex SpineData::get_slot_location(std::string slotname)
{
	spBone *staff_tip;
	staff_tip = spSkeleton_findBone(skeleton, slotname.c_str());

	t_vertex return_data;

	if(flip)
		return_data = t_vertex(-staff_tip->worldX*0.006, (staff_tip->worldY*0.006)-1.5, 0);
	else
		return_data = t_vertex(staff_tip->worldX*0.006, (staff_tip->worldY*0.006) - 1.5, 0);

	return return_data;
}

void SpineData::update_skeleton(float time_delta)
{
	spAnimation_apply(spSkeletonData_findAnimation(skeletonData, animation_name), skeleton, current_time-start_time/550, (current_time-start_time+time_delta)/550, looping, NULL, NULL);
	spSkeleton_updateWorldTransform(skeleton);

	// this only probably needs to happen when animation changes
	setslots();

	current_time = SDL_GetTicks();
}