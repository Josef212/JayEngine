#include "Mesh.h"



Mesh::Mesh(GameObject* gObj) : Component(gObj)
{
	type = MESH;
}


Mesh::~Mesh()
{
}

void Mesh::enable()
{
	if (!active)
		active = true;
}

void Mesh::disable()
{
	if (active)
		active = false;
}

void Mesh::init()
{

}

void Mesh::update(float dt)
{

}

void Mesh::cleanUp()
{

}

void Mesh::loadToOpenGl()
{

}