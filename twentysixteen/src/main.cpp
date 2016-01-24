#pragma comment(lib, "x86/SDL2")
#pragma comment(lib, "x86/SDL2main")
#pragma comment(lib, "x86/SDL2_ttf")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "GLU32")
#pragma comment(lib, "SOIL")
#pragma comment(lib, "spine-c")

#define NO_SDL_GLEXT

// General Libraries
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

// Local Headers
#include "techdemos\ttftechdemo.h"
#include "techdemos\spinetechdemo.h"

// and a few globals
bool done = 0; // Quit?
SDL_Window *window;
Level *current_level;

std::map<levels, Level*> level_map;


void init_opengl()
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, res_width, res_height);
	glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	gluPerspective(80, res_width / res_height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);  // Select The Model View Matrix
	glLoadIdentity();    // Reset The Model View Matrix

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();

	current_level->draw();

	SDL_GL_SwapWindow(window);
}

void handle_sdl_event()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_QUIT)
			done = true;
	}

}

void init_levels()
{
	level_map[TECHDEMO_BASE] = new BaseTechDemo();
	level_map[TECHDEMO_SPINE] = new SpineTechDemo();
	level_map[TECHDEMO_TTF] = new TTFTechDemo();

	for (auto it = std::begin(level_map); it != std::end(level_map); ++it)
	{
		it->second->init();
	}

}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_AUDIO);
	SDL_Init(SDL_INIT_JOYSTICK);

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	window = SDL_CreateWindow("TwentySixteen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, res_width, res_height, SDL_WINDOW_OPENGL);
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	SDL_ShowCursor(0);

	init_opengl();

	SDL_JoystickEventState(SDL_ENABLE);
	SDL_Joystick *joy = SDL_JoystickOpen(0);

	// Initialize Paintrbush (fonts)
	Paintbrush::init();

	// set up level map
	init_levels();

	// Load levels and set the current level
	current_level = level_map[TECHDEMO_BASE];

	while (!done)
	{
		handle_sdl_event();
		current_level->run();
		draw();
	}

	// shut everything down
	//SDL_CloseAudio();

	TTF_Quit();
	SDL_GL_DeleteContext(glcontext);
	SDL_JoystickClose(joy);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}