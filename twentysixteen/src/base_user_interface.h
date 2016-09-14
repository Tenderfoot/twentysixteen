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

class UIWidget
{
public:
	UIWidget()
	{
		visible = true;
	}

	float x, y, width, height;
	bool visible;
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

	float x, y, width, height;
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
	float x, y, width, height;
	bool visible;
	void draw();
};

class BaseUserInterface
{
public:

	std::vector<UIWidget*> widgets;

	void draw();
	void add_widget(UIWidget *new_widget);

};

