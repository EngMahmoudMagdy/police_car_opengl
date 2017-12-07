#define _CRT_SECURE_NO_DEPRECATE

#include<windows.h>
#include<gl/freeglut.h>
#include<gl/glaux.h>
#include <string.h>
#   pragma comment (lib, "glaux.lib")    /* link with Win32 GLUT lib */
#include <stdio.h>
#include <share.h>
#include<iostream>
#include"ObjModel.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std ;
   ObjModel model;
   unsigned int id ;
   float basevertices[] =
   {
	   10, -7,
	   -10, -7,
	   -10, -10,
	   10, -10
   };
   GLfloat lightColor[] = { 0.5f,0.5f,0.5f,1.0f };
   GLfloat lightPos[] = { 8.0f,0,8.0f,1.0f };
GLuint ltexture, basetextureid;
#pragma warning(diable:4996)
const float FLOOR_TEXTURE_SIZE = 50.0f; //The size of each floor "tile"
float _guyPos = 0;
void CreateTexture(UINT &texture, LPSTR strFileName)
{
	AUX_RGBImageRec *pBitmap = NULL;
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	glGenTextures(1, &texture);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}

void charKey(unsigned char k,int x,int y)
{
	glutPostRedisplay();
	switch(k)
	{
	case 27:
		exit(0);
	break;

	}
}
void drawText(const char *text, GLfloat x, GLfloat y)
{
	//glBegin(GL_2D);
	//glutSwapBuffers();
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRasterPos3f(x, y,0);
	for (int i = 0; i < strlen(text); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
	//glEnd();
}
GLuint loadTexture(const char * filename)
{
	GLuint texture;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned char * data;
	int width, height;
	FILE * file;
	file = fopen(filename, "rb");
	if (!file)
	{
		printf("Image could not be opened\n");
		return NULL;
	}
	// Read the header, i.e. the 54 first bytes
	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return NULL;
	}

	/*if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct2 BMP file\n");    return false; }*/
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct2 BMP file\n");    return NULL; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return  texture;
}
float angle = 0;
void drawbase(float vertices1[], GLuint texture)
{

	glTranslatef(0.0f, 0.0f, -80.0f);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, (2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, -1000.0f);
	glEnd();
	
}

void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	angle+=0.3;
	glTranslatef(0,-20,-80);
	glScalef(0.5,0.5,0.5);
	
	glRotatef(angle, 0,angle,1);
	glRotatef(0, 1, 0, 0);
	if(model.hasTextureCoord())
	{
	glBindTexture(GL_TEXTURE_2D,id);
	glEnable(GL_TEXTURE_2D);
	}
	glBegin(GL_TRIANGLES);
	{
		for(int i=0;i<model.vectorOfFace.size();i++)
		{
	
	    if(model.hasNormalCoord())
			glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].xn,
					   model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].yn,
					   model.vectorOfNormal[model.vectorOfFace[i].firstVertex[2]].zn);
		if(model.hasTextureCoord())
			    glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].firstVertex[1]].s,
		 				 model.vectorOfTexture[model.vectorOfFace[i].firstVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].x,
					   model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].y,
					   model.vectorOfVertex[model.vectorOfFace[i].firstVertex[0]].z);
		 if(model.hasNormalCoord())
			glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].xn,
					   model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].yn,
					   model.vectorOfNormal[model.vectorOfFace[i].secondVertex[2]].zn);
		 if(model.hasTextureCoord())
			 glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].secondVertex[1]].s,
		 				 model.vectorOfTexture[model.vectorOfFace[i].secondVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].x,
					   model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].y,
					   model.vectorOfVertex[model.vectorOfFace[i].secondVertex[0]].z);
		
		 if(model.hasNormalCoord())
			 glNormal3d(model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].xn,
					   model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].yn,
					   model.vectorOfNormal[model.vectorOfFace[i].thirdVertex[2]].zn);

		 if(model.hasTextureCoord()) 
			 glTexCoord2f(model.vectorOfTexture[model.vectorOfFace[i].thirdVertex[1]].s,
		 			   model.vectorOfTexture[model.vectorOfFace[i].thirdVertex[1]].t);
			glVertex3f(model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].x,
					   model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].y,
					   model.vectorOfVertex[model.vectorOfFace[i].thirdVertex[0]].z);
		}
	}
	glEnd();
	
	drawbase(basevertices, basetextureid);
	

	glColor3f(1, 1, 1);
	drawText("Hello world", -0.9f, 0.9f);
	//text
	glutPostRedisplay();
	glutSwapBuffers();
	//Update _guyPos
	_guyPos += 15;
	while (_guyPos > FLOOR_TEXTURE_SIZE) {
		_guyPos -= FLOOR_TEXTURE_SIZE;
	}
	glFlush();
}

void reshape (int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	//gluOrtho2D(0,w,0,h);
	gluPerspective(65,1,.1,-100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }
int main(int argc, char** argv)
{

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE |GLUT_DEPTH| GLUT_RGB);
   glutInitWindowSize (400, 400); 
   glutInitWindowPosition (100, 100);
   
   model.load("policecar.obj");
   glutCreateWindow ("wavefront file loader ");
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);	
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   glColor3f(1,1,1);
   glClearColor(0,0,0,1);
   CreateTexture(id, "policecar2.bmp");
   basetextureid = loadTexture("asphalt.bmp");
   
   glutDisplayFunc(display); 

   glutReshapeFunc(reshape);
   glutIgnoreKeyRepeat(0);
   glutKeyboardFunc(charKey);


   glutMainLoop();
   return 0;
}
