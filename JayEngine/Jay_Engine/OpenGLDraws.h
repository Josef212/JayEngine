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

void cubeWithTriVertices()
{
	//Draw a cube using triangles
	glBegin(GL_TRIANGLES);

	float s = 1.f / 2;

	//----------------------- Front
	glVertex3d(s, s, s);
	glVertex3d(-s, -s, s);
	glVertex3d(s, -s, s);

	glVertex3d(-s, s, s);
	glVertex3d(-s, -s, s);
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
}

//----------------------------------------------------

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

