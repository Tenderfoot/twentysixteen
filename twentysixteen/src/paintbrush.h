#pragma once

/* Class Paintbrush */
// Class Paintbrush will act as a wrapper for some
// of the libraries, and a simple way to store and 
// access loaded data.

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <SOIL/SOIL.h>
#include <gl/GLU.h>
#include <gl/gl.h>     // The GL Header File
#include <map>
#include <fstream>

#include "common.h"
#include "model_data.h"
#include "linear_algebra.h"
#include "light_manager.h"

// shader stuff
#define uglGetProcAddress(x) wglGetProcAddress(x)
#define WIN32_OR_X11

typedef struct
{
	GLuint vertex_buffer;
	GLuint texcoord_buffer;
	GLuint color_buffer;

	float *verticies;
	float *texcoords;
	float *colors;

	int num_faces;
	GLuint texture;

}t_VBO;

class Paintbrush
{
public:
	// initialize fonts...
	static TTF_Font *font;
	static void init();
	// for shaders:
	static void setup_extensions();

	// VBO stuff
	static t_VBO create_vbo(t_3dModel model);

	// draw a unit length quad at origin
	static void draw_quad();
	static void draw_vbo(t_VBO the_vbo);

	static int num_faces;

	// Convert a string into an OpenGL Texture using SDL_TTF
	static GLuint TextToTexture(GLubyte r, GLubyte g, GLubyte b, const char* text, int ptsize);

	// Load a texture
	static GLuint Soil_Load_Texture(std::string filename, bool for_assimp);

	static void draw_text(std::string text, float x, float y, float width, float height);

	// TODO get rid of this and set it up so rendered strings are stored
	static GLuint font_texture;

	// texturedb stores and returns already loaded
	// textures or rendered strings
	// string -> texture GLuint
	static std::map<std::string, GLuint> texture_db;
	static GLuint get_texture(std::string texture_id, bool text, bool flip);

	// draw a 3d model
	static void draw_model(t_3dModel *mymodel);
	static void draw_face(t_face face_to_draw, GLuint texture);
	static void draw_cube();

	// shader stuff
	static GLenum load_shader(std::string shadername);

	static void use_shader(GLenum shader);
	static void stop_shader();
	static void set_uniform(GLenum shader, std::string uniform_name, float data);
	static void set_uniform_location(GLenum shader, GLint uniform_location, float data);
	static int get_uniform_location(GLenum shader, std::string variable_name);

	// I'm going to need a static map of all the shaders, name->shader, for storage and recall
	// I will also need a std::map<std::pair<GLenum, char*>, GLint> -> this will map <shader,uniform>->uniform location
	static std::map<std::string, GLenum> shader_db;
	static GLenum get_shader(std::string shader_id);
	static std::map<std::pair<GLenum, std::string>, GLint> uniform_db;
	static GLint get_uniform(GLenum shader, std::string uniform_name);
	static void update_shader_uniforms();

	// draw collisiongroups
	static void draw_collision_group(t_collisiongroup group, float distance);
};

