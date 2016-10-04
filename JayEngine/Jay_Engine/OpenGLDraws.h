#pragma once
#include "OpenGL.h"

#define PI 3.14159265359f

//------------------------------------------------

void drawLine()
{
	//Draw a line
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();
	glLineWidth(1.0f);
}

//--------------------------------------------------
//Draw a cube using direct mode vertices
void cubeWithTriVertices(uint texId)
{
	//Draw a cube using triangles
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLES);

	float s = 1.f / 2;

	//----------------------- Front
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(s, -s, s);

	glTexCoord2f(1.f, 0.f);
	glVertex3d(-s, s, s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	//----------------------- Top
	glVertex3d(-s, s, s);
	glVertex3d(s, s, s);
	glVertex3d(s, s, -s);

	glVertex3d(-s, s, s);
	glVertex3d(s, s, -s);
	glVertex3d(-s, s, -s);
	//----------------------- Right
	glVertex3d(s, s, s);
	glVertex3d(s, -s, s);
	glVertex3d(s, s, -s);

	glVertex3d(s, -s, s);
	glVertex3d(s, -s, -s);
	glVertex3d(s, s, -s);
	//----------------------- Left
	glVertex3d(-s, s, -s);
	glVertex3d(-s, -s, s);
	glVertex3d(-s, s, s);

	glVertex3d(-s, s, -s);
	glVertex3d(-s, -s, -s);
	glVertex3d(-s, -s, s);
	//----------------------- Bottom
	glVertex3d(-s, -s, s);
	glVertex3d(s, -s, -s);
	glVertex3d(s, -s, s);

	glVertex3d(-s, -s, s);
	glVertex3d(-s, -s, -s);
	glVertex3d(s, -s, -s);
	//----------------------- Behind
	glVertex3d(s, s, -s);
	glVertex3d(s, -s, -s);
	glVertex3d(-s, s, -s);

	glVertex3d(s, -s, -s);
	glVertex3d(-s, -s, -s);
	glVertex3d(-s, s, -s);

	
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);//Clean buffer
	glDisable(GL_TEXTURE_2D);
}

//----------------------------------------------------
//Load a cube using a buffer of vertices
void loadVerticesBuffer(uint* vArray)
{
	glGenBuffers(1, (GLuint*)vArray);
	glBindBuffer(GL_ARRAY_BUFFER, *vArray);
	float vertices[108] =
	{ 0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, 
		0.5f, 0.5f, -0.5f,

		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,

		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,

		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f
	
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, vertices, GL_STATIC_DRAW);
}

//Draw a cube using a buffer of vertices prevously loaded
void drawFromVerticesArray(uint vArray, int jump, int arraySize)
{
	//Draw a cube using vertex array
	glBegin(GL_VERTEX_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vArray);
	glVertexPointer(jump, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, arraySize);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnd();
}

//------------------------------------------------
//Load a cube using vertices and indices in two buffers
void loadVerticesIndices(uint* vArrayId, uint* iArrayId)
{
	float s = 0.5;
	float v[12] = {
		s, s, s,
		s, -s, s,
		-s, -s, s,
		-s, s, s
	};

	uint i[6]{
		0, 2, 1, 0, 3, 2
	};

	glGenBuffers(1, (GLuint*)vArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, *vArrayId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, v, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)iArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iArrayId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, i, GL_STATIC_DRAW);
}

//Draw a cube using a buffer of vertices and a buffer of indices prevously loaded
void drawVerticesIndex(uint vArrayId, uint iArrayId)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vArrayId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iArrayId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

//-------------------------------------------


//-------------------------------------------
void drawSphere(float radius, int slices, int traces)
{
	glBegin(GL_POINTS);

	for (int i = 0; i < slices + 1; i++)
	{
		float angleY = PI*i / traces;
		float y = cos(angleY);
		float xz = sin(angleY);

		for (int j = 0; j < slices + 1; j++)
		{

			float angleX = 2 * PI*j / slices;
			float x = xz*cos(angleX);
			float z = xz*sin(angleX);
			glVertex3d(x, y, z);
		}
	}

	glEnd();
}

//-------------------------------------------

void drawCylinder(float r, float h)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i+=20)
	{
		float a = i * PI / 180;
		glVertex3d(r * cos(a), -h* 0.5f, r * sin(a));
	}
	glEnd();

	glBegin(GL_POLYGON);

	glNormal3f(0.f, 0.f, 0.f);

	for (int i = 0; i < 360; i += 20)
	{
		float a = i * PI / 180;
		glVertex3d(r * cos(a), h* 0.5f, r * sin(a));
	}

	glEnd();
}

