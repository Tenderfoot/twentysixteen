#pragma comment(lib, "x86/SDL2")
#pragma comment(lib, "x86/SDL2main")
#pragma comment(lib, "x86/SDL2_mixer")
#pragma comment(lib, "x86/SDL2_ttf")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "GLU32")
#pragma comment(lib, "SOIL")
#pragma comment(lib, "spine-c")
#pragma comment(lib, "assimp-vc140-mt")

#define NO_SDL_GLEXT

// whats next


// General Libraries
#include <GL/glew.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

// Local Headers
#include "techdemos\ttftechdemo.h"
#include "techdemos\spinetechdemo.h"
#include "techdemos\audiotechdemo.h"
#include "techdemos\modeltechdemo.h"
#include "techdemos\shadertechdemo.h"
#include "techdemos\modeltopolygontechdemo.h"
#include "techdemos\grasstechdemo.h"
#include "techdemos\scenetechdemo.h"
#include "techdemos\emittertechdemo.h"
#include "techdemos\physicstechdemo.h"
#include "techdemos\lighttechdemo.h"
#include "techdemos\postprocessdemo.h"
#include "techdemos\enemytechdemo.h"
#include "techdemos\graveyardtechdemo.h"
#include "techdemos\meadowtechdemo.h"
#include "techdemos\marshtechdemo.h"
#include "techdemos\towntechdemo.h"
#include "techdemos\dungeontechdemo.h"
#include "techdemos\breakouttechdemo.h"
#include "techdemos\JokeTechDemo.h"
#include "techdemos\fogofwartechdemo.h"

// and a few globals
bool done = 0; // Quit?
SDL_Window *window;
Level *current_level;
SDL_Joystick *joy;

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
	gluPerspective(90, res_width / res_height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);  // Select The Model View Matrix
	glLoadIdentity();    // Reset The Model View Matrix

	glClearColor(0.05f, 0.05f, 0.05f, 0.5f);
}

void get_mouse_in_space()
{
	// refresh mouse in space - needs to happen after draw
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)current_level->mousex;
	winY = (float)viewport[3] - (float)current_level->mousey;
	glReadPixels(winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	current_level->mouse_in_space = t_vertex(posX, posY, posZ);
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();

	// draw level
	current_level->draw();

	// get mouse position in space
	// this needs to happen after draw, because it uses
	// glReadPixels to get the depth at the mouse cursor,
	// the uses gluUnProject to determine the position at that depth.
	get_mouse_in_space();

	// draw HUD stuff
	current_level->draw_hud();

	SDL_GL_SwapWindow(window);
}

boundinput translate_key_input(SDL_Keycode keycode)
{
	switch (keycode)
	{
		case SDLK_SPACE:
			return ACTION;
		case SDLK_s:
			return DOWN;
		case SDLK_a:
			return LEFT;
		case SDLK_d:
			return RIGHT;
		case SDLK_w:
			return UP;
		case SDLK_ESCAPE:
			return BACK;
		case SDLK_PAGEUP:
			return NEXT;
		case SDLK_PAGEDOWN:
			return PREVIOUS;
		case SDLK_F12:
			return EDITOR_SAVE;
		case SDLK_F1:
			return EDITOR_CREATE_MODE;
		case SDLK_F2:
			return EDITOR_EDIT_MODE;
		case SDLK_F3:
			return EDITOR_PLAY_MODE;
		case SDLK_DELETE:
			return EDITOR_DELETE;
		case SDLK_q:
			return QUICKSWITCH;
		case SDLK_e:
			return USE;
		case SDLK_b:
			return HOTKEY_B;
		case SDLK_KP_7:
			return EDITOR_SCALE_X_PLUS;
		case SDLK_KP_4:
			return EDITOR_SCALE_X_MINUS;
		case SDLK_KP_8:
			return EDITOR_SCALE_Y_PLUS;
		case SDLK_KP_5:
			return EDITOR_SCALE_Y_MINUS;
		case SDLK_KP_9:
			return EDITOR_SCALE_Z_PLUS;
		case SDLK_KP_6:
			return EDITOR_SCALE_Z_MINUS;
		case SDLK_KP_1:
			return EDITOR_RED;
		case SDLK_KP_2:
			return EDITOR_GREEN;
		case SDLK_KP_3:
			return EDITOR_BLUE;
		case SDLK_t:
			return EDITOR_T;
	}

	return NO_BIND;
}

boundinput translate_joy_input(int joybutton, bool hat)
{
	if (hat)
	{
		switch (joybutton)
		{
			case 1:
				return UP;
				break;
			case 2:
				return RIGHT;
				break;
			case 4:
				return DOWN;
				break;
			case 8:
				return LEFT;
				break;
			case 0:
				return HAT_CLEAR;
				break;
		}
		return NO_BIND;
	}
	else
	{
		switch (joybutton)
		{
			
			case 0:
				return ACTION;
				break;
			case 1:
				return BACK;
				break;
			case 2:
				return EDITOR_T;
				break;
			case 5:
				return R_SHOULDER;
				break;
		}
	}
}

void handle_sdl_event()
{
	SDL_Event event;
	bool was_keypress = false;
	bool was_joypress = false;
	bool type;

	while (SDL_PollEvent(&event))
	{
		// Translate and send key events
		if (event.type == SDL_KEYDOWN)
			current_level->take_input(translate_key_input(event.key.keysym.sym), true);

		if (event.type == SDL_KEYUP)
			current_level->take_input(translate_key_input(event.key.keysym.sym), false);

		// Translate and send joypad events
		if (event.type == SDL_JOYBUTTONDOWN)
			current_level->take_input(translate_joy_input(event.jbutton.button, false), true);

		if (event.type == SDL_JOYBUTTONUP)
			current_level->take_input(translate_joy_input(event.jbutton.button, false), false);

		if (event.type == SDL_JOYHATMOTION)
		{
			current_level->take_input(translate_joy_input(SDL_JoystickGetHat(joy, 0), true), true);
		}

		if (event.type == SDL_MOUSEMOTION)
		{	
			current_level->mousex = (float)event.motion.x;
			current_level->mousey =  (float)event.motion.y;
			current_level->mouse_relative = t_vertex(event.motion.xrel, event.motion.yrel, 0.0f);
			current_level->take_input(MOUSEMOTION, false);
		}

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
				current_level->take_input(LMOUSE, true);

			if (event.button.button == SDL_BUTTON_RIGHT)
				current_level->take_input(RMOUSE, true);		

			if (event.button.button == SDL_BUTTON_MIDDLE)
				current_level->take_input(MIDDLEMOUSE, true);
		}

		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				current_level->take_input(LMOUSE, false);

			if (event.button.button == SDL_BUTTON_RIGHT)
				current_level->take_input(RMOUSE, false);
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0)
				current_level->take_input(MWHEELUP, true);

			if (event.wheel.y < 0)
				current_level->take_input(MWHEELDOWN, true);
		}


		// quit event
		if (event.type == SDL_QUIT || event.type == SDL_QUIT)
			done = true;
	}

}

void init_levels()
{
	level_map[TECHDEMO_BASE] = new BaseTechDemo();
	level_map[TECHDEMO_SPINE] = new SpineTechDemo();
	level_map[TECHDEMO_TTF] = new TTFTechDemo();
	level_map[TECHDEMO_AUDIO] = new AudioTechDemo();
	level_map[TECHDEMO_MODEL] = new ModelTechDemo();
	level_map[TECHDEMO_SHADER] = new ShaderTechDemo();
	level_map[TECHDEMO_MODELTOPOLY] = new ModelToPolygonTechDemo();
	level_map[TECHDEMO_GRASS] = new GrassTechDemo();
	level_map[TECHDEMO_SCENE] = new SceneTechDemo();
	level_map[TECHDEMO_EMITTER] = new EmitterTechDemo();
	level_map[TECHDEMO_PHYSICS] = new PhysicsTechDemo();
	level_map[TECHDEMO_LIGHTS] = new LightTechDemo();
	level_map[TECHDEMO_POSTPROCESS] = new PostProcessDemo();
	level_map[TECHDEMO_ENEMY] = new EnemyTechDemo();
	level_map[TECHDEMO_MARSH] = new MarshTechDemo();
	level_map[TECHDEMO_GRAVEYARD] = new GraveyardTechDemo();
	level_map[TECHDEMO_MEADOW] = new MeadowTechDemo();
	level_map[TECHDEMO_TOWN] = new TownTechDemo();
	level_map[TECHDEMO_DUNGEON] = new DungeonTechDemo();
	level_map[TECHDEMO_BREAKOUT] = new BreakoutTechDemo();
	level_map[TECHDEMO_JOKE] = new JokeTechDemo();
	level_map[TECHDEMO_FOGOFWAR] = new FogOfWarTechDemo();

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

	SDL_ShowCursor(1);

	init_opengl();

	SDL_JoystickEventState(SDL_ENABLE);
	joy = SDL_JoystickOpen(0);

	// Initialize Paintrbush (fonts) and AudioController
	Paintbrush::init();
	AudioController::init();
	LightManager::init();

	// set up level map
	init_levels();

	// set start level
	current_level = level_map[TECHDEMO_BASE];
	current_level->init();
	current_level->initialized = true;

	float previous_time = SDL_GetTicks();
	// main loop
	while (!done)
	{

		Paintbrush::update_shader_uniforms();
		handle_sdl_event();
		
		// Run
		float current_time = SDL_GetTicks();
		current_level->run(current_time - previous_time);
		previous_time = current_time;
		
		// Draw
		draw();

		// Level change request?
		if (current_level->exit_level != LEVEL_NONE)
		{
			levels next_level; 
			next_level = current_level->exit_level;

			if (next_level == QUIT)
				done = true;
			else
			{
				current_level->exit_level = LEVEL_NONE;
				current_level = level_map[next_level];
				if (current_level->initialized == false)
				{
					current_level->init();
					current_level->initialized = true;
				}
				current_level->reset();
			}
		}
	}

	// shut everything down
	SDL_CloseAudio();
	TTF_Quit();
	SDL_GL_DeleteContext(glcontext);
	SDL_JoystickClose(joy);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}