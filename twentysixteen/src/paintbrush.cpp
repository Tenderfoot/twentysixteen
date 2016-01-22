
#include <stdio.h>
#include "paintbrush.h"

GLuint Paintbrush::font_texture = 0;
TTF_Font *Paintbrush::font = NULL;

void Paintbrush::init()
{
	font = TTF_OpenFont("data/fonts/Greyscale Basic Bold.ttf", 100);
	if (!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

	font_texture = TextToTexture(255,255,255,"Test this shit", 14);
}

void Paintbrush::draw_quad()
{
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.99f);	glVertex3f(0.5f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, 0.99f);	glVertex3f(-0.5f, 0.5f, 0.0f);
			glTexCoord2f(0.0f, -0.01f);	glVertex3f(-0.5f, -0.5f, 0.0f);
			glTexCoord2f(1.0f, -0.01f);	glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();
	glPopMatrix();
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, msg->w, msg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, msg->pixels);

	//fontwidth = msg->w;
	//fontheight = msg->h;

	SDL_FreeSurface(msg);
	return font_texture;
}