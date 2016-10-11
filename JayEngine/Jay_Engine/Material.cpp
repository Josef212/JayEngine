#include "Material.h"



Material::Material(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MATERIAL;
	name.assign("Material");
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