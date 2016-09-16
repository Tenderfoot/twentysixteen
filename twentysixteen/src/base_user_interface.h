#pragma once

/* Class BaseUserInterface */
// This class will be inherited by the Title_Menu level,
// as well as the BaseGameLevel (to draw the game HUD).
// BaseUserInterface is a collection of UIWidget subclasses,
// each one drawn. The screen coordinates are stored as
// percentages so that it scales properly with different
// resolutions.

#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <SOIL/SOIL.h>
#include <vector>

#include "paintbrush.h"
#include "grid_manager.h"
#include "gridcharacter.h"

class UIWidget
{
public:
	UIWidget()
	{
		visible = true;
		absorbs_mouse = false;
	}

	float x, y, width, height;
	bool visible;
	bool absorbs_mouse;

	virtual bool coords_in_ui(t_vertex mousecoords)
	{
		if (mousecoords.x > this->x*res_width - (0.5*this->width*res_width) && mousecoords.x < this->x*res_width + (0.5*this->width*res_width) &&
			mousecoords.y > this->y*res_height - (0.5*this->height*res_height) && mousecoords.y < this->y*res_height + (0.5*this->height*res_height) && absorbs_mouse)
		{
			return true;
		}
		return false;
	}

	t_vertex color;
	int index;

	virtual void draw() = 0;
};

class TextWidget : public UIWidget
{
public:

	TextWidget(std::string text, float x, float y, float width, float height)
	{
		this->text = text;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	std::string text;
	void draw();
};

class UIImage : public UIWidget
{
public:
	UIImage(float x, float y, float width, float height, GLuint texture)
	{
		this->texture = texture;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	GLuint texture;
	void draw();
};

class ListWidget : public UIWidget
{
public:
	ListWidget(std::vector<char*> list_items) : list_items(list_items) {};

	void set_data(float x, float y, float width, float height, bool visible)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->visible = visible;
	}

	void next_item()
	{
		current_selection++;
		if (current_selection >= list_items.size())
			current_selection = 0;
	}

	void previous_item()
	{
		current_selection--;
		if (current_selection < 0)
			current_selection = list_items.size()-1;
	}

	std::vector<char*> list_items;
	int current_selection;
	void draw();
};

class MapWidget : public UIWidget
{
public:
	MapWidget(GridManager *grid)
	{
		visible = true;
		map_grid = grid;
		x = res_width/100;
		y = (res_height / 9)*7.5;
		width = res_width / 14;
		height = (res_height / 12);
	}

	GridManager *map_grid;
	bool visible;
	void draw();
};

class AbilityButton : public UIWidget
{
public:

	AbilityButton(float x, float y, float width, float height, GLuint image, int index)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->tex = image;
		absorbs_mouse = true;
		this->index = index;
		this->active = false;
	}

	bool coords_in_ui(t_vertex mousecoords)
	{
		if (mousecoords.x > this->x*res_width - (0.5*this->width*res_width) && mousecoords.x < this->x*res_width + (0.5*this->width*res_width) &&
			mousecoords.y > this->y*res_height - (0.5*this->height*res_height) && mousecoords.y < this->y*res_height + (0.5*this->height*res_height))
		{
			color = t_vertex(1.0f, 0.0f, 1.0f);
			return true;
		}
		color = t_vertex(0.5f, 0.5f, 0.5f);
		return false;
	}

	GLuint tex;
	bool visible;
	bool active;
	void draw();
};

class CharacterWidget : public UIWidget
{
public:
	CharacterWidget(GridCharacter *newchar)
	{
		visible = true;
		character = newchar;
		x = res_width / 2;
		y = res_height / 2;
		width = res_width / 4;
		height = (res_height / 4);
	}

	GridCharacter *character;
	bool visible;
	void draw();
};

class CombatLog : public UIWidget
{
public:
	CombatLog(std::vector<std::string> *log_address)
	{
		visible = true;
		log = log_address;
		x = res_width / 8;
		y = (res_height / 16) * 11;
		width = res_width / 4;
		height = (res_height / 4);
	}

	std::vector<std::string> *log;
	bool visible;
	void draw();
};



class BaseUserInterface
{
public:

	std::vector<UIWidget*> widgets;
	t_vertex mouse_coords;

	int mouse_focus();

	void draw();
	void add_widget(UIWidget *new_widget);
};

