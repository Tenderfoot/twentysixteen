
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

// texture storage
GLuint Paintbrush::font_texture = 0;
TTF_Font *Paintbrush::font = NULL;
std::map<char*, GLuint, cmp_str> Paintbrush::texture_db = {};

// shaders and uniform storage
std::map<char*, GLenum, cmp_str> Paintbrush::shader_db = {};
std::map<std::pair<GLenum, char*>, GLint> Paintbrush::uniform_db = {};

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
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
		uglGetProcAddress("glUniform1fARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
		uglGetProcAddress("glUniform1iARB");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)
		uglGetProcAddress("glGetShaderiv");
}

GLenum Paintbrush::get_shader(char* shader_id)
{
	std::map<char*, GLenum, cmp_str>::iterator it;

	it = shader_db.find(shader_id);

	if (it == shader_db.end())
	{
		char *new_string = new char[128];
		strcpy_s(new_string, sizeof(char) * 128, shader_id);

		shader_db.insert({ new_string, load_shader(shader_id) });
	}

	return shader_db[shader_id];
}

GLint Paintbrush::get_uniform(GLenum shader, char* uniform_name)
{
	std::map<std::pair<GLenum, char *>, GLint, cmp_str>::iterator it;
	std::pair<GLenum, char*> mypair = std::make_pair(shader, uniform_name);
	GLint return_value;

	// I tried to overload the comparator so I could use map.find
	// but it wasn't cooperating with the custom operator I had
	// so I'm rewriting it to
	for (auto it = uniform_db.begin(); it != uniform_db.end(); ++it)
	{

		if (shader == it->first.first)
		{
			if (strcmp(it->first.second, uniform_name) == 0)
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

GLenum Paintbrush::load_shader(char *shadername)
{
	GLenum shader_program;

	shader_program = glCreateProgramObjectARB();

	GLenum my_fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	GLenum my_vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

	GLint myUniformLocation = glGetUniformLocationARB(shader_program, "myUniform");
	glUniform1fARB(myUniformLocation, 2.0f);


	// LOAD IN FRAGMENT SHADER
	char *full_path = new char[64];
	sprintf_s(full_path, sizeof(char) * 64, "data/shaders/%s.frag", shadername);

	std::ifstream myfile(full_path);
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
	sprintf_s(full_path, sizeof(char) * 64, "data/shaders/%s.vert", shadername);
	std::ifstream myfiletwo(full_path);
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

GLint Paintbrush::get_uniform_location(GLenum shader, char *variable_name)
{
	glUseProgramObjectARB(shader);
	GLint loc = glGetUniformLocationARB(shader, variable_name);
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

void Paintbrush::set_uniform(GLenum shader, char* uniform_name, float data)
{
	set_uniform_location(shader, get_uniform(shader, uniform_name), data);
}

void Paintbrush::update_shader_uniforms()
{
	// Iterate through each shader and send time
	std::map<char*, GLenum, cmp_str>::iterator it;

	for (auto it = shader_db.begin(); it != shader_db.end(); ++it)
	{
		set_uniform(it->second, "Time", ((float)SDL_GetTicks())/1000);
		set_uniform(it->second, "num_lights", LightManager::lights.size());

		int i;
		for (i = 0; i < LightManager::lights.size(); i++)
		{
			char *uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char)*255, "scene_lights[%d].radius", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].radius);
			
			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].x", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].x);

			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].y", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].y);

			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].z", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].z);

			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].r", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].r);

			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].g", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].g);

			uniform_name = new char[256];
			sprintf_s(uniform_name, sizeof(char) * 255, "scene_lights[%d].b", i);
			set_uniform(it->second, uniform_name, LightManager::lights[i].b);
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

GLuint Paintbrush::Soil_Load_Texture(char *filename, bool for_assimp)
{
	GLuint loaded_texture;
	int flags;

	if (for_assimp)
		flags = SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y;
	else
		flags = SOIL_FLAG_MIPMAPS;

	loaded_texture = SOIL_load_OGL_texture
		(	filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			flags);

	// Make sure texture is set to repeat on wrap
	glBindTexture(GL_TEXTURE_2D, loaded_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

void Paintbrush::draw_text(char *text, float x, float y, float width, float height)
{
	// draw the text
	glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glScalef(width, height, 1.0f);

		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(text, true, false));
		Paintbrush::draw_quad();
	glPopMatrix();
}

GLuint Paintbrush::get_texture(char* texture_id, bool text, bool flip)
{
	std::map<char*, GLuint, cmp_str>::iterator it;

	it = texture_db.find(texture_id);

	if (it == texture_db.end())
	{
		char *new_string = new char[128];
		strcpy_s(new_string, sizeof(char) * 128, texture_id);

		if (text)
		{
			texture_db.insert({ new_string, TextToTexture(255, 255, 255, texture_id, 14) });
		}
		else
		{
			texture_db.insert({ new_string, Soil_Load_Texture(texture_id, flip) });
		}
	}

	return texture_db[texture_id];
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
	std::vector<t_edge> *edge_set;
	for (i = 0; i < group.collision_groups.size(); i++)
	{
		edge_set = &group.collision_groups.at(i);

		for (auto it = edge_set->begin(); it != edge_set->end(); ++it)
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