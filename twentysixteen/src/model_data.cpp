#include "model_data.h"
#include "paintbrush.h"

Assimp::Importer ModelData::importer;

t_3dModel *ModelData::import(char *filename, float scale) 
{
	const aiScene *scene;
	char *dir;

	t_3dModel *new_model = new t_3dModel;

	dir = new char[256];
	sprintf_s(dir, sizeof(char) * 256, "data/models/%s", filename);

	scene = importer.ReadFile(dir, aiProcess_PreTransformVertices);

	// If the import failed, report it
	if (!scene)
	{
		printf("%s", importer.GetErrorString());
		return false;
	}
	// Now we can access the file's contents. 
	printf("success? %d\n", scene->mNumMeshes);

	int num, i, j;

	num = scene->mNumMaterials;

	printf("Number of Materials: %d\n", num);

	for (i = 0; i<num; i++)
	{
		aiString name;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &name, NULL, NULL, NULL, NULL, NULL);
		printf("key: %s\n", name.C_Str());

		char filename[80];

		if (name.C_Str()[0] == 'X')
			printf("\n");

		dir = new char[256];

		sprintf_s(dir, sizeof(char) * 256, "data/models/%s", name.C_Str());
		printf("%s\n", dir);

		new_model->textures.push_back(Paintbrush::get_texture(dir, false));

		glBindTexture(GL_TEXTURE_2D, new_model->textures.at(new_model->textures.size() - 1));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	t_mesh *new_mesh;
	t_face *new_face;
	t_vertex temp;

	for (i = 0; i<scene->mNumMeshes; i++)
	{
		new_mesh = new t_mesh();
		for (j = 0; j<scene->mMeshes[i]->mNumFaces; j++)
		{
			new_face = new t_face();
			new_face->material_index = scene->mMeshes[i]->mMaterialIndex;

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				temp.texcoord_x = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[0]].x;
				temp.texcoord_y = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[0]].y;
			}

			temp.x = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].x*scale;
			temp.y = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].y*scale;
			temp.z = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[0]].z*scale;

			new_face->verticies.push_back(temp);


			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				temp.texcoord_x = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[1]].x;
				temp.texcoord_y = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[1]].y;
			}

			temp.x = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].x*scale;
			temp.y = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].y*scale;
			temp.z = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].z*scale;

			new_face->verticies.push_back(temp);

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				temp.texcoord_x = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[2]].x;
				temp.texcoord_y = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[2]].y;
			}

			temp.x = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].x*scale;
			temp.y = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].y*scale;
			temp.z = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[2]].z*scale;

			new_face->verticies.push_back(temp);

			temp.x = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].x;
			temp.y = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].y;
			temp.z = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[2]].z;

			new_face->normal = temp;

			new_mesh->faces.push_back(new_face);
		}
		new_model->meshes.push_back(new_mesh);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	new_model->filename = filename;

	//loaded_models.push_back(new_model);

	return new_model;
}