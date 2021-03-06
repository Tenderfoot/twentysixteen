#include "model_data.h"
#include "paintbrush.h"

Assimp::Importer ModelData::importer;

t_3dModel *ModelData::import(std::string filename, float scale) 
{
	const aiScene *scene;
	t_3dModel *new_model = new t_3dModel;
	std::string dir;

	scene = importer.ReadFile(filename, aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices | aiProcess_Triangulate);

	// If the import failed, report it
	if (!scene)
	{

		printf("%s", importer.GetErrorString());
			return false;
	}
	
	// Now we can access the file's contents. 

	int num, i, j;
	num = scene->mNumMaterials;

	for (i = 0; i<num; i++)
	{
		aiString name;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &name, NULL, NULL, NULL, NULL, NULL);
		if (name.C_Str()[0] == 'X')
			printf("\n");

		dir = "";
		dir.append("data/models/");
		dir.append(name.C_Str());

		std::string test = name.C_Str();

		if (test == "grass.jpg")
		{
			new_model->grass_index = i;
		}

		new_model->textures.push_back(Paintbrush::get_texture(dir, false, true));

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

			temp.x = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].x;
			temp.y = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].y;
			temp.z = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[0]].z;

			new_face->normal[0] = temp;

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				temp.texcoord_x = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[1]].x;
				temp.texcoord_y = scene->mMeshes[i]->mTextureCoords[0][scene->mMeshes[i]->mFaces[j].mIndices[1]].y;
			}

			temp.x = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].x*scale;
			temp.y = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].y*scale;
			temp.z = scene->mMeshes[i]->mVertices[scene->mMeshes[i]->mFaces[j].mIndices[1]].z*scale;

			new_face->verticies.push_back(temp);

			temp.x = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].x;
			temp.y = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].y;
			temp.z = scene->mMeshes[i]->mNormals[scene->mMeshes[i]->mFaces[j].mIndices[1]].z;

			new_face->normal[1] = temp;

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

			new_face->normal[2] = temp;

			new_mesh->faces.push_back(new_face);
		}
		new_model->meshes.push_back(new_mesh);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	new_model->filename = filename;

	return new_model;
}