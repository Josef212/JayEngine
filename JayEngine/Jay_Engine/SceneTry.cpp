#include "Application.h"
#include "Globals.h"
#include "SceneTry.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleInput.h"
#include "UI_Inspector.h"
#include "ModuleEditor.h"

#include "OpenGLDraws.h"

SceneTry::SceneTry(bool startEnabled) : Module(startEnabled)
{
}


SceneTry::~SceneTry()
{
}

bool SceneTry::init()
{
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

	return true;
}

update_status SceneTry::preUpdate(float dt)
{
	if (app->input->getKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		gO = createGameObject();
		app->editor->inspector->selectGameObject(gO);
	}

	return UPDATE_CONTINUE;
}

update_status SceneTry::udate(float dt)
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

	return UPDATE_CONTINUE;
}

bool SceneTry::cleanUp()
{
	return true;
}

GameObject* SceneTry::createGameObject()
{
	GameObject* ret = NULL;

	ret = new GameObject(NULL);

	return ret;
}