#include "Application.h"
#include "Globals.h"
#include "SceneTry.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleInput.h"
#include "UI_Inspector.h"
#include "ModuleEditor.h"
#include "ModuleImporter.h"

#include "OpenGL.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

//#include "OpenGLDraws.h"

SceneTry::SceneTry(bool startEnabled) : Module(startEnabled)
{
}


SceneTry::~SceneTry()
{
}

bool SceneTry::init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool SceneTry::start()
{//TMP: create a cube with an array
	//loadVerticesBuffer(&vArray);
	/*float s = 0.5;
	float v[12] = {
	s, s, s,
	s, -s, s,
	-s, -s, s,
	-s, s, s
	};

	uint i[6]{
	0, 2, 1, 0, 3, 2
	};

	glGenBuffers(1, (GLuint*) &verticesId);
	glBindBuffer(GL_ARRAY_BUFFER, verticesId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, v, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &indicesId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, i, GL_STATIC_DRAW);*/

	//Create checker texture
	/*const int CHECKERS_H = 64, CHECKERS_W = 64;
	GLubyte checkImage[CHECKERS_H][CHECKERS_W][4];
	for (int i = 0; i < CHECKERS_H; i++)
	{
		for (int j = 0; j < CHECKERS_W; j++)
		{
			int c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texId);
	glBindBuffer(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_W, CHECKERS_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/

	app->importer->loadFBXs("Assets/fbx/MechaT.fbx");
	//texId = ilutGLLoadImage("Assets/textures/Diffuse_Mech.png");
	texId = ilutGLLoadImage("Assets/textures/Lenna.png");

	return true;
}

update_status SceneTry::preUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status SceneTry::update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status SceneTry::postUpdate(float dt)
{
	///cubeWithTriVertices(texId);

	//drawFromVerticesArray(vArray, 3, 108);

	//drawSphere(1, 25, 25);

	//drawCylinder(1, 3);

	//------------------------------

	/*glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, verticesId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);*/

	//------------------------------

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glBegin(GL_TRIANGLES);

	float s = 1.f * 2;

	//----------------------- Front
	
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);	
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);	
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, -s, s);

	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, s, s);	
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, s);	
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	//----------------------- Top

	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, s, s);
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, s, -s);

	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, s, s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, s, -s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, s, -s);
	//----------------------- Right

	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(s, -s, s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, s, -s);

	glTexCoord2f(0.f, 1.f);
	glVertex3d(s, -s, s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(s, -s, -s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, s, -s);
	//----------------------- Left

	glTexCoord2f(1.f, 0.f);
	glVertex3d(-s, s, -s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, -s, s);
	glTexCoord2f(1.f, 1.f);
	glVertex3d(-s, s, s);

	glTexCoord2f(1.f, 0.f);
	glVertex3d(-s, s, -s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, -s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, -s, s);
	//----------------------- Bottom
	
	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, -s, s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, -s, -s);
	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, -s, s);

	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, -s, s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, -s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, -s, -s);
	//----------------------- Behind

	glTexCoord2f(1.f, 1.f);
	glVertex3d(s, s, -s);
	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, -s, -s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, s, -s);

	glTexCoord2f(1.f, 0.f);
	glVertex3d(s, -s, -s);
	glTexCoord2f(0.f, 0.f);
	glVertex3d(-s, -s, -s);
	glTexCoord2f(0.f, 1.f);
	glVertex3d(-s, s, -s);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	app->importer->drawMeshes(app->importer->Mmeshes);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	return UPDATE_CONTINUE;
}

bool SceneTry::cleanUp()
{
	ilShutDown();
	return true;
}