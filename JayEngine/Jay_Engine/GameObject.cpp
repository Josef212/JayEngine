#include "Application.h"
#include "GameObject.h"

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

#include "OpenGL.h"

#include "ModuleManager.h"


GameObject::GameObject(GameObject* parent, int id) : parent(parent), id(id)
{
	name.assign("Game Object");
	init();
	addComponent(TRANSFORMATION);
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
}

void GameObject::update(float dt)
{
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->update(dt);
	}
	for (uint j = 0; j < components.size(); ++j)
	{
		components[j]->update(dt);
	}

	if (isGOActive())
	{
		draw();
	}
}

void GameObject::cleanUp()
{
	for (uint i = 0; i < childrens.size(); ++i)
	{
		childrens[i]->cleanUp();
	}
	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->cleanUp();
	}
}

const char* GameObject::getName()const
{
	return name.c_str();
}

void GameObject::setName(const char* str)
{
	if (str)
		name.assign(str);
}

int GameObject::getGOId()const
{
	return id;
}

bool GameObject::isGOActive()
{
	return goActive;
}

void GameObject::setGOEnable(bool set)
{
	goActive = set;
}

Component* GameObject::addComponent(ComponentType type)
{
	Component* ret = NULL;

	switch (type)
	{
		case UNKNOWN:
			_LOG("Error while creating component, no component type.");
		break;

		case TRANSFORMATION:
		{
			ret = new Transform(this, nextCompId);
		}
		break;

		case MESH:
		{
			ret = new Mesh(this, nextCompId);
		}
		break;

		case MATERIAL:
		{
			ret = new Material(this, nextCompId);
		}
		break;
	}

	if (ret)
	{
		++nextCompId;
		components.push_back(ret);
	}

	return ret;
}

GameObject* GameObject::addChild()
{
	GameObject* ret = NULL;

	ret = new GameObject(this, app->manager->nextGOId);
	childrens.push_back(ret);
	++app->manager->nextGOId;

	return ret;
}

bool GameObject::removeComponent(Component* comp)
{
	bool ret = false;

	if (comp)
	{
		uint pos = 0;
		for (; pos < components.size(); ++pos)
		{
			if (components[pos] == comp)
			{
				ret = true;
				break;
			}
		}
		if (ret)
		{
			components[pos]->cleanUp();
			components.erase(components.begin() + pos);
		}
	}
	else
		ret = false;

	return ret;
}

Component* GameObject::findComponent(ComponentType type)
{
	Component* ret = NULL;

	for (uint i = 0; i < components.size(); ++i)
	{
		if(components[i]->type == type)
			return components[i];
	}

	return ret;
}

//std::vector<Component*> findComponent(ComponentType type);

/*const std::vector<Component*> GameObject::getComponents()const
{
	return components;
}*/

GameObject* GameObject::getParent() const
{
	return parent;
}

void GameObject::draw()
{
	Transform* trans = (Transform*)findComponent(TRANSFORMATION);
	Mesh* mesh = (Mesh*)findComponent(MESH);
	Material* mat = (Material*)findComponent(MATERIAL);

	if (trans && mesh)
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (mesh->renderWireframe)
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.f);
			glColor4f(0.f, 0.8f, 0.8f, 1.f);
			if (app->manager->getSelected() == this)
			{
				glLineWidth(0.1f);
				glColor4f(0.2f, 1.f, 0.2f, 1.f);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}
		else
		{
			//RenderNormals

			if (mesh->renderNormals)
			{
				glDisable(GL_LIGHTING);
				glLineWidth(0.7f);
				glBegin(GL_LINES);
				glColor4f(0.54f, 0.f, 0.54f, 1.f);

				for (uint i = 0; i < mesh->numVertices; ++i)
				{
					glVertex3f(mesh->vertices[i * 3], mesh->vertices[i * 3 + 1], mesh->vertices[i * 3 + 2]);
					glVertex3f(mesh->vertices[i * 3] + mesh->normals[i * 3], mesh->vertices[i * 3 + 1] + mesh->normals[i * 3 + 1], mesh->vertices[i * 3 + 2] + mesh->normals[i * 3 + 2]);
				}

				glEnd();
				glLineWidth(1.f);
				glEnable(GL_LIGHTING);
			}

			//-------------

			glEnable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (mat)
			{
				//glColor4f(mat->getColor().r, mat->getColor().g, mat->getColor().b, mat->getColor().a);
				uint tex = mat->idTexture;
				if (tex > 0)
				{
					glBindTexture(GL_TEXTURE_2D, tex);
				}
			}
			else
			{
				glColor4f(0.5f, 0.5f, 0.5f, 1.f);
			}

			if (mesh->idTexCoords > 0)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			}

			if (mesh->idNormals > 0)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);
		glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);

		//Cleaning
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//------
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//==============
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}