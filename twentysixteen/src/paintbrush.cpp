
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "paintbrush.h"

// binding methods from extenions
PFNGLCREATEPROGRAMOBJECTARBPROC     glCreateProgramObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC            glDeleteObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC      glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC            glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC           glCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC    glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC            glAttachObjectARB = NULL;
PFNGLGETINFOLOGARBPROC              glGetInfoLogARB = NULL;
PFNGLLINKPROGRAMARBPROC             glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC        glUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC      glGetUniformLocationARB = NULL;
PFNGLUNIFORM1FARBPROC               glUniform1fARB = NULL;
PFNGLUNIFORM1IARBPROC               glUniform1iARB = NULL;
PFNGLGETSHADERIVPROC                glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = NULL;

// stuff for VBOs...
PFNGLGENBUFFERSARBPROC      glGenBuffersARB = NULL;
PFNGLBUFFERDATAARBPROC      glBufferDataARB = NULL;
PFNGLBINDBUFFERARBPROC      glBindBufferARB = NULL;


// texture storage
GLuint Paintbrush::font_texture = 0;
TTF_Font *Paintbrush::font = NULL;
std::map<std::string, GLuint> Paintbrush::texture_db = {};

int Paintbrush::num_faces = 0;
GLuint Paintbrush::mesh_vbo[] = { 0,0,0 };

// shaders and uniform storage
std::map<std::string, GLenum> Paintbrush::shader_db = {};
std::map<std::pair<GLenum, std::string>, GLint> Paintbrush::uniform_db = {};

void Paintbrush::init()
{
	font = TTF_OpenFont("data/fonts/Greyscale Basic Bold.ttf", 100);
	if (!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	
	setup_extensions();
}


void Paintbrush::setup_extensions()
{
	char* extensionList = (char*)glGetString(GL_EXTENSIONS);

	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)
		uglGetProcAddress("glCreateProgramObjectARB");
	glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)
		uglGetProcAddress("glDeleteObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)
		uglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)
		uglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)
		uglGetProcAddress("glCompileShaderARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)
		uglGetProcAddress("glGetObjectParameterivARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)
		uglGetProcAddress("glAttachObjectARB");
	
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)
		uglGetProcAddress("glGetShaderInfoLog");
	
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)
		uglGetProcAddress("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)
		uglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)
		uglGetProcAddress("glUseProgramObjectARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)
		uglGetProcAddress("glGetUniformLocationARB");

	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)
		uglGetProcAddress("glGenBuffersARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC)
		uglGetProcAddress("glBufferDataARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC)
		uglGetProcAddress("glBindBufferARB");

	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
		uglGetProcAddress("glUniform1fARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
		uglGetProcAddress("glUniform1iARB");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)
		uglGetProcAddress("glGetShaderiv");
}

void Paintbrush::create_vbo(t_3dModel model)
{
	//Create a new VBO and use the variable id to store the VBO id
	glGenBuffersARB(3, &mesh_vbo[0]);

	float *verticies;
	float *colors;
	float *texcoords;


	int i, j;
	num_faces = 0;
	for (j = 0; j < model.meshes.size(); j++)
	{
		num_faces += model.meshes.at(j)->faces.size();
	}

	verticies = new float[num_faces * 3 * 3];
	colors = new float[num_faces * 3 * 3];
	texcoords = new float[num_faces * 3 * 2];
	
	// set verticies, colors and texcoords to the appropriate values
	int vert_index_last = 0;
	for (j = 0; j < model.meshes.size(); j++)
	{
		for (i = 0; i < model.meshes.at(j)->faces.size(); i++)
		{
			verticies[(vert_index_last*9)] =	 model.meshes.at(j)->faces.at(i)->verticies.at(0).x;
			verticies[(vert_index_last * 9) + 1] = model.meshes.at(j)->faces.at(i)->verticies.at(0).y;
			verticies[(vert_index_last * 9) + 2] = model.meshes.at(j)->faces.at(i)->verticies.at(0).z;
			verticies[(vert_index_last * 9) + 3] = model.meshes.at(j)->faces.at(i)->verticies.at(1).x;
			verticies[(vert_index_last * 9) + 4] = model.meshes.at(j)->faces.at(i)->verticies.at(1).y;
			verticies[(vert_index_last * 9) + 5] = model.meshes.at(j)->faces.at(i)->verticies.at(1).z;
			verticies[(vert_index_last * 9) + 6] = model.meshes.at(j)->faces.at(i)->verticies.at(2).x;
			verticies[(vert_index_last * 9) + 7] = model.meshes.at(j)->faces.at(i)->verticies.at(2).y;
			verticies[(vert_index_last * 9) + 8] = model.meshes.at(j)->faces.at(i)->verticies.at(2).z;

			colors[(vert_index_last * 9) + 0] = 1.0f;
			colors[(vert_index_last * 9) + 1] = 1.0f;
			colors[(vert_index_last * 9) + 2] = 1.0f;
			colors[(vert_index_last * 9) + 3] = 1.0f;
			colors[(vert_index_last * 9) + 4] = 1.0f;
			colors[(vert_index_last * 9) + 5] = 1.0f;
			colors[(vert_index_last * 9) + 6] = 1.0f;
			colors[(vert_index_last * 9) + 7] = 1.0f;
			colors[(vert_index_last * 9) + 8] = 1.0f;

			texcoords[(vert_index_last * 6)] = model.meshes.at(j)->faces.at(i)->verticies.at(0).texcoord_x;
			texcoords[(vert_index_last * 6) + 1] = model.meshes.at(j)->faces.at(i)->verticies.at(0).texcoord_y;
			texcoords[(vert_index_last * 6) + 2] = model.meshes.at(j)->faces.at(i)->verticies.at(1).texcoord_x;
			texcoords[(vert_index_last * 6) + 3] = model.meshes.at(j)->faces.at(i)->verticies.at(1).texcoord_y;
			texcoords[(vert_index_last * 6) + 4] = model.meshes.at(j)->faces.at(i)->verticies.at(2).texcoord_x;
			texcoords[(vert_index_last * 6) + 5] = model.meshes.at(j)->faces.at(i)->verticies.at(2).texcoord_y;

			vert_index_last += 1;

		}
	}
	//Make the new VBO active
	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[0]);
	//Upload vertex data to the video device
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*num_faces * 3 * 3, verticies, GL_STATIC_DRAW);

	//Make the new VBO active
	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[1]);
	//Upload vertex data to the video device
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*num_faces * 3 * 3, colors, GL_STATIC_DRAW);

	//Make the new VBO active
	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[2]);
	//Upload vertex data to the video device
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*num_faces * 3 * 2, texcoords, GL_STATIC_DRAW);

	//Make the new VBO active. Repeat here incase changed since initialisation
	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[1]);
	glColorPointer(3, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER, mesh_vbo[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Paintbrush::draw_vbo()
{
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture("data/models/meadow_atlas.png", false, true));
		glDrawArrays(GL_TRIANGLES, 0, num_faces * 3);
	glPopMatrix();
}

GLenum Paintbrush::get_shader(std::string shader_id)
{
	std::map<std::string, GLenum>::iterator it;

	it = shader_db.find(shader_id);

	if (it == shader_db.end())
	{
		shader_db.insert({ shader_id, load_shader(shader_id) });
	}

	return shader_db[shader_id];
}

GLint Paintbrush::get_uniform(GLenum shader, std::string uniform_name)
{
	std::map<std::pair<GLenum, std::string>, GLint>::iterator it;
	std::pair<GLenum, std::string> mypair = std::make_pair(shader, uniform_name);
	GLint return_value;

	// I tried to overload the comparator so I could use map.find
	// but it wasn't cooperating with the custom operator I had
	// so I'm rewriting it to
	for (auto it = uniform_db.begin(); it != uniform_db.end(); ++it)
	{

		if (shader == it->first.first)
		{
			if (it->first.second == uniform_name)
			{
				return it->second;
			}
		}
	}
	
	return_value = get_uniform_location(shader, uniform_name);

	if (return_value != -1)
	{
		uniform_db.insert({ std::make_pair(shader, uniform_name), return_value });
	}

	return return_value;
}

// streak

GLenum Paintbrush::load_shader(std::string shadername)
{
	GLenum shader_program;

	shader_program = glCreateProgramObjectARB();

	GLenum my_fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	GLenum my_vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

	std::ostringstream full_path;
	full_path << "data/shaders/" << shadername << ".frag";

	std::ifstream myfile(full_path.str().c_str());
	std::stringstream ss;
	if (myfile.is_open())
	{
		ss << myfile.rdbuf();
		myfile.close();
	}
	else
	{
		printf("file not found\n");
		return 0;
	}

	std::string test(ss.str().c_str());
	const GLchar *frag_shad_src = test.c_str();
	glShaderSourceARB(my_fragment_shader, 1, &frag_shad_src, NULL);

	// LOAD IN VERTEX SHADER
	full_path.str("");
	full_path << "data/shaders/" << shadername << ".vert";

	std::ifstream myfiletwo(full_path.str().c_str());
	std::stringstream sstwo;
	if (myfiletwo.is_open())
	{
		sstwo << myfiletwo.rdbuf();
		myfiletwo.close();
	}
	std::string testtwo(sstwo.str().c_str());
	const GLchar *vertex_shad_src = testtwo.c_str();
	glShaderSourceARB(my_vertex_shader, 1, &vertex_shad_src, NULL);

	// Compile The Shaders
	int i;
	// VERTEX
	glCompileShaderARB(my_vertex_shader);

	GLint maxLength = 0;
	glGetShaderiv(my_vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
	if (maxLength > 0)
	{
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(my_vertex_shader, maxLength, &maxLength, &errorLog[0]);
		printf("///////// SHADER COMPILER /////////////\n");
		for (i = 0; i<errorLog.size(); i++)
		{
			printf("%c", errorLog.at(i));
		}
		printf("///////// END SHADER COMPILER /////////////\n");
	}

	// FRAGMENT
	glCompileShaderARB(my_fragment_shader);

	maxLength = 0;
	glGetShaderiv(my_fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);
	if (maxLength > 0)
	{
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(my_fragment_shader, maxLength, &maxLength, &errorLog[0]);
		printf("///////// SHADER COMPILER /////////////\n");
		for (i = 0; i<errorLog.size(); i++)
		{
			printf("%c", errorLog.at(i));
		}
		printf("///////// END SHADER COMPILER /////////////\n");
	}

	// Attach The Shader Objects To The Program Object
	glAttachObjectARB(shader_program, my_vertex_shader);
	glAttachObjectARB(shader_program, my_fragment_shader);

	glLinkProgramARB(shader_program);

	return shader_program;
}

void Paintbrush::use_shader(GLenum shader)
{
	glUseProgramObjectARB(shader);
}

void Paintbrush::stop_shader()
{
	glUseProgramObjectARB(0);
}

GLint Paintbrush::get_uniform_location(GLenum shader, std::string variable_name)
{
	glUseProgramObjectARB(shader);
	GLint loc = glGetUniformLocationARB(shader, variable_name.c_str());
	glUseProgramObjectARB(0);

	return loc;
}

void Paintbrush::set_uniform_location(GLenum shader, GLint uniform_location, float data)
{
	glUseProgramObjectARB(shader);
	if (uniform_location != -1)
	{
		glUniform1fARB(uniform_location, data);
	}
	glUseProgramObjectARB(0);
}

void Paintbrush::set_uniform(GLenum shader, std::string uniform_name, float data)
{
	set_uniform_location(shader, get_uniform(shader, uniform_name), data);
}

void Paintbrush::update_shader_uniforms()
{
	// Iterate through each shader and send time
	std::map<std::string, GLenum>::iterator it;

	for (auto it = shader_db.begin(); it != shader_db.end(); ++it)
	{
		set_uniform(it->second, "Time", ((float)SDL_GetTicks())/1000);
		set_uniform(it->second, "num_lights", LightManager::lights.size());

		int i;

		std::ostringstream variable_to_set;

		for (i = 0; i < LightManager::lights.size(); i++)
		{
			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].radius";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].radius);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].x";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].x);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].y";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].y);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].z";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].z);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].r";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].r);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].g";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].g);

			variable_to_set.str("");
			variable_to_set << "scene_lights[" << i << "].b";
			set_uniform(it->second, variable_to_set.str(), LightManager::lights[i].b);
		}
	}

}

void Paintbrush::draw_quad()
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.00f);	glVertex3f(0.5f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, 1.00f);	glVertex3f(-0.5f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(-0.5f, -0.5f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();
	glPopMatrix();
}

GLuint Paintbrush::Soil_Load_Texture(std::string filename, bool for_assimp)
{
	GLuint loaded_texture;
	int flags;

	if (for_assimp)
		flags = SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y;
	else
		flags = SOIL_FLAG_MIPMAPS;

	loaded_texture = SOIL_load_OGL_texture
		(	filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			flags);

	// Make sure texture is set to repeat on wrap
	glBindTexture(GL_TEXTURE_2D, loaded_texture);

	if (for_assimp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		// make sure it doesn't wrap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	return loaded_texture;
}

GLuint Paintbrush::TextToTexture(GLubyte r, GLubyte g, GLubyte b, const char* text, int ptsize)
{
	SDL_Color color = { r, g, b };
	SDL_Surface* msg = TTF_RenderText_Blended(font, text, color);

	// create new texture, with default filtering state (==mipmapping on)
	glGenTextures(1, &font_texture);
	glBindTexture(GL_TEXTURE_2D, font_texture);

	// disable mipmapping on the new texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// make sure it doesn't wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, msg->w, msg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, msg->pixels);

	//fontwidth = msg->w;
	//fontheight = msg->h;

	SDL_FreeSurface(msg);
	return font_texture;
}

void Paintbrush::draw_text(std::string text, float x, float y, float width, float height)
{
	// draw the text
	glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(width, height, 1.0f);

		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(text, true, false));
		Paintbrush::draw_quad();
	glPopMatrix();
}

GLuint Paintbrush::get_texture(std::string texture_id, bool text, bool flip)
{
	std::map<std::string, GLuint>::iterator it;
	
	it = texture_db.find(texture_id);
	
	if (it == texture_db.end())
	{
		if (text)
		{
			texture_db.insert({ texture_id, TextToTexture(255, 255, 255, texture_id.c_str(), 14) });
		}
		else
		{
			texture_db.insert({ texture_id, Soil_Load_Texture(texture_id, flip) });
		}
	}

	return texture_db[texture_id];
}

void Paintbrush::draw_face(t_face face_to_draw, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);

	int i, j = 0;

	//TODO don't use highest possible anistro
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

	float alpha = 1.0f;

	glEnable(GL_DEPTH_TEST);

	t_vertex temp;
	t_vertex temp2;
	t_vertex cross;

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		glEnable(GL_BLEND);
		glPushMatrix();

		glBegin(GL_TRIANGLES);

			glNormal3f(face_to_draw.normal[0].x, face_to_draw.normal[0].y, face_to_draw.normal[0].z);
			glTexCoord2f(face_to_draw.verticies.at(0).texcoord_x, face_to_draw.verticies.at(0).texcoord_y);
			glVertex3f(face_to_draw.verticies.at(0).x, face_to_draw.verticies.at(0).y, face_to_draw.verticies.at(0).z);

			glNormal3f(face_to_draw.normal[1].x, face_to_draw.normal[1].y, face_to_draw.normal[1].z);
			glTexCoord2f(face_to_draw.verticies.at(1).texcoord_x, face_to_draw.verticies.at(1).texcoord_y);
			glVertex3f(face_to_draw.verticies.at(1).x, face_to_draw.verticies.at(1).y, face_to_draw.verticies.at(1).z);

			glNormal3f(face_to_draw.normal[2].x, face_to_draw.normal[2].y, face_to_draw.normal[2].z);
			glTexCoord2f(face_to_draw.verticies.at(2).texcoord_x, face_to_draw.verticies.at(2).texcoord_y);
			glVertex3f(face_to_draw.verticies.at(2).x, face_to_draw.verticies.at(2).y, face_to_draw.verticies.at(2).z);

		glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);

	glPopMatrix();
}

void Paintbrush::draw_model(t_3dModel *mymodel)
{
	glEnable(GL_TEXTURE_2D);

	int i, j = 0;

	//TODO don't use highest possible anistro
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

	float alpha = 1.0f;

	glEnable(GL_DEPTH_TEST);

	t_vertex temp;
	t_vertex temp2;
	t_vertex cross;

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();

	for (i = 0; i<mymodel->meshes.size(); i++)
	{
		for (j = 0; j<mymodel->meshes.at(i)->faces.size(); j++)
		{
			glBindTexture(GL_TEXTURE_2D, mymodel->textures[mymodel->meshes.at(i)->faces.at(j)->material_index]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
			glEnable(GL_BLEND);

			glPushMatrix();

				glBegin(GL_TRIANGLES);

					glNormal3f(mymodel->meshes.at(i)->faces.at(j)->normal[0].x, mymodel->meshes.at(i)->faces.at(j)->normal[0].y, mymodel->meshes.at(i)->faces.at(j)->normal[0].z);
					glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).texcoord_y);
					glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).z);

					glNormal3f(mymodel->meshes.at(i)->faces.at(j)->normal[1].x, mymodel->meshes.at(i)->faces.at(j)->normal[1].y, mymodel->meshes.at(i)->faces.at(j)->normal[1].z);
					glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).texcoord_y);
					glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).z);

					glNormal3f(mymodel->meshes.at(i)->faces.at(j)->normal[2].x, mymodel->meshes.at(i)->faces.at(j)->normal[2].y, mymodel->meshes.at(i)->faces.at(j)->normal[2].z);
					glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).texcoord_y);
					glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).z);

				glEnd();

			glPopMatrix();

			glDisable(GL_BLEND);
		}
	}

	glPopMatrix();
}

void Paintbrush::draw_collision_group(t_collisiongroup group, float distance)
{
	int i;
	t_polygon *edge_set;
	for (i = 0; i < group.collision_groups.size(); i++)
	{
		edge_set = &group.collision_groups.at(i);

		for (auto it = edge_set->edges.begin(); it != edge_set->edges.end(); ++it)
		{
			glBindTexture(GL_TEXTURE_2D, NULL);
			glColor3f(1.0f, 0.0f, 1.0f);
			glBegin(GL_LINES);
				glVertex3f(it->verticies.at(0).x, it->verticies.at(0).y, distance);
				glVertex3f(it->verticies.at(1).x, it->verticies.at(1).y, distance);
			glEnd();
		}
	}
	
}

void Paintbrush::draw_cube()
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, get_texture("data/images/greybrick.png", false, false));

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-1.0f, 1.0f, -1.0f);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}