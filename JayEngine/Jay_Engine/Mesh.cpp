#include "Mesh.h"



Mesh::Mesh(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MESH;
	name.assign("Mesh");
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