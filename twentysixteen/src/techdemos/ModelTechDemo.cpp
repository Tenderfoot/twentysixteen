
#include "modeltechdemo.h"

void ModelTechDemo::init()
{
	TechDemoUI.add_widget(new TextWidget("Model Tech Demo", 0.5, 0.2, 0.5, 0.3));
	TechDemoUI.add_widget(new TextWidget("Press ESCAPE to go back", 0.5, 0.9, 0.5, 0.05));
	mymodel = ModelData::import("just_tree.3DS", 0.002);

}

void ModelTechDemo::run()
{
}

void ModelTechDemo::take_input(boundinput input, bool type)
{
	if (input == BACK && type == true)
		exit_level = TECHDEMO_BASE;
}

void ModelTechDemo::draw()
{
	BaseTechDemo::draw();

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		int i, j = 0;
		//TODO don't use highest possible anistro
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

		glTranslatef(0.0f, -20.0f, -50.0f);

		float alpha = 1.0f;

		glEnable(GL_DEPTH_TEST);

		t_vertex temp;
		t_vertex temp2;
		t_vertex cross;

		glColor3f(1.0f, 1.0f, 1.0f);

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
