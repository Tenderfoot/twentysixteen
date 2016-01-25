
#include <stdio.h>
#include "paintbrush.h"

GLuint Paintbrush::font_texture = 0;
TTF_Font *Paintbrush::font = NULL;
std::map<char*, GLuint, cmp_str> Paintbrush::texture_db = {};

void Paintbrush::init()
{
	font = TTF_OpenFont("data/fonts/Greyscale Basic Bold.ttf", 100);
	if (!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
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

GLuint Paintbrush::Soil_Load_Texture(char *filename)
{
	GLuint loaded_texture;

	loaded_texture = SOIL_load_OGL_texture
		(	filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS	);

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

		glBindTexture(GL_TEXTURE_2D, Paintbrush::get_texture(text, true));
		Paintbrush::draw_quad();
	glPopMatrix();
}

GLuint Paintbrush::get_texture(char* texture_id, bool text)
{
	std::map<char*, GLuint, cmp_str>::iterator it;
	
	int i;

	it = texture_db.find(texture_id);

	if (it == texture_db.end())
	{
		char *new_string = new char[128];
		strcpy_s(new_string, sizeof(char) * 128, texture_id);

		if (text)
		{
			texture_db.insert({ new_string ,TextToTexture(255, 255, 255, texture_id, 14) });
		}
		else
		{
			texture_db.insert({ new_string , Soil_Load_Texture(texture_id) });
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

			//glNormal3f(mymodel->meshes.at(i)->faces.at(j)->normal.x,mymodel->meshes.at(i)->faces.at(j)->normal.y,mymodel->meshes.at(i)->faces.at(j)->normal.z);

			temp.x = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).x - mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).x;
			temp.y = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).y - mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).y;
			temp.z = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).z - mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).z;

			temp2.x = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).x - mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).x;
			temp2.y = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).y - mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).y;
			temp2.z = mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).z - mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).z;

			cross.x = temp.y*temp2.z - temp.z*temp2.y;
			cross.y = temp.z*temp2.x - temp.x*temp2.z;
			cross.z = temp.x*temp2.y - temp.y*temp2.x;

			glNormal3f(cross.x, cross.y, cross.z);

			//glColor4f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).r, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).g, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).b, alpha);
			glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).texcoord_y);
			glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(0).z);

			//glColor4f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).r, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).g, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).b, alpha);
			glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).texcoord_y);
			glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(1).z);

			//glColor4f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).r, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).g, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).b, alpha);
			glTexCoord2f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).texcoord_x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).texcoord_y);
			glVertex3f(mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).x, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).y, mymodel->meshes.at(i)->faces.at(j)->verticies.at(2).z);

			glEnd();
			glPopMatrix();

			glDisable(GL_BLEND);
		}
	}

	glPopMatrix();
}