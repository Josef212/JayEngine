#include "Material.h"



Material::Material(GameObject* gObj) : Component(gObj)
{
	type = MATERIAL;
}


Material::~Material()
{
}

void Material::enable()
{
	if (!active)
		active = true;
}

void Material::disable()
{
	if (active)
		active = false;
}

void Material::init()
{

}

void Material::update(float dt)
{

}

void Material::cleanUp()
{

}