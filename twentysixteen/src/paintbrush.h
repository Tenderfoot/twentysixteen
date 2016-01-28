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

// shader stuff
#define uglGetProcAddress(x) wglGetProcAddress(x)
#define WIN32_OR_X11

class Paintbrush
{
public:
	// initialize fonts...
	static TTF_Font *font;
	static void init();
	// for shaders:
	static void setup_extensions();

	// draw a unit length quad at origin
	static void draw_quad();

	// Convert a string into an OpenGL Texture using SDL_TTF
	static GLuint TextToTexture(GLubyte r, GLubyte g, GLubyte b, const char* text, int ptsize);

	// Load a texture
	static GLuint Soil_Load_Texture(char *filename, bool for_assimp);

	static void draw_text(char *text, float x, float y, float width, float height);

	// TODO get rid of this and set it up so rendered strings are stored
	static GLuint font_texture;

	// texturedb stores and returns already loaded
	// textures or rendered strings
	// string -> texture GLuint
	static std::map<char*, GLuint, cmp_str> texture_db;
	static GLuint get_texture(char* texture_id, bool text, bool flip);

	// draw a 3d model
	static void draw_model(t_3dModel *mymodel);
	static void draw_cube();

	// assimp needs invert_y soil parameter....
	static GLenum load_shader(char *shadername);

	static void use_shader(GLenum shader);
	static void stop_shader();

};

