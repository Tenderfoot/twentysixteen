#pragma once

/* Class Paintbrush */
// Class Paintbrush will act as a wrapper for some
// of the libraries, and a simple way to store and 
// access loaded data.

#include <SDL_opengl.h>
#include <SDL_ttf.h>

class Paintbrush
{
public:
	// initialize fonts...
	static TTF_Font *font;
	static void init();

	// draw a unit length quad at origin
	static void draw_quad();

	// Convert a string into an OpenGL Texture using SDL_TTF
	static GLuint TextToTexture(GLubyte r, GLubyte g, GLubyte b, const char* text, int ptsize);

	// TODO get rid of this and set it up so rendered strings are stored
	static GLuint font_texture;
};

