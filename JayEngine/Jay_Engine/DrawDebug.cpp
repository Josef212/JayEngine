#include "OpenGL.h"
#include "Globals.h"
#include "DrawDebug.h"

//-------------------------------------

void beginDebugDraw()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void endDebugDraw()
{
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

//-------------------------------------

void drawBoxDebug(AABB& box)
{

}

void drawSphereDebug()
{

}

void drawFrustumDebug(Frustum& frustum)
{
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	glColor4f(1.f, 1.f, 0.f, 1.f);

	glBegin(GL_QUADS);

	glVertex3fv((float*)&vertices[1]);
	glVertex3fv((float*)&vertices[5]);
	glVertex3fv((float*)&vertices[7]);
	glVertex3fv((float*)&vertices[3]);
						   
	glVertex3fv((float*)&vertices[4]);
	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[2]);
	glVertex3fv((float*)&vertices[6]);
						   
	glVertex3fv((float*)&vertices[5]);
	glVertex3fv((float*)&vertices[4]);
	glVertex3fv((float*)&vertices[6]);
	glVertex3fv((float*)&vertices[7]);
						   
	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[1]);
	glVertex3fv((float*)&vertices[3]);
	glVertex3fv((float*)&vertices[2]);
						   
	glVertex3fv((float*)&vertices[3]);
	glVertex3fv((float*)&vertices[7]);
	glVertex3fv((float*)&vertices[6]);
	glVertex3fv((float*)&vertices[2]);
						   
	glVertex3fv((float*)&vertices[0]);
	glVertex3fv((float*)&vertices[4]);
	glVertex3fv((float*)&vertices[5]);
	glVertex3fv((float*)&vertices[1]);

	glEnd();
}