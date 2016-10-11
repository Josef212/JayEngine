#include "Mesh.h"


#include "OpenGL.h"

//TMP: will change assimp for importer in order to use my own format
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"


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

bool Mesh::loadMesh(aiScene* scene, aiMesh* mesh, bool loadToRAM)
{
	bool ret = false;

	if (mesh)
	{
		if(mesh->mName.C_Str() != '\0')
			setName(mesh->mName.C_Str());
		
		numVertices = mesh->mNumVertices;
		vertices = new float[numVertices * 3];
		memcpy(vertices, mesh->mVertices, sizeof(float)*numVertices * 3);
		_LOG("New mesh called %s with %d vertices.", getName(), numVertices);

		if (mesh->HasFaces())
		{
			numIndices = mesh->mNumFaces * 3;
			indices = new uint[numIndices]; //Assume each face is a triangle
			for (uint j = 0; j < mesh->mNumFaces; ++j)
			{
				if (mesh->mFaces[j].mNumIndices != 3)
				{
					_LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&indices[j * 3], mesh->mFaces[j].mIndices, sizeof(uint) * 3);
				}
			}
			_LOG("Mesh %s has %d indices.", getName(), numIndices);
		}

		if (mesh->HasNormals())
		{
			numNormals = numVertices * 3;
			normals = new float[numNormals];
			memcpy(normals, mesh->mNormals, sizeof(float) * numNormals);
		}

		if (mesh->HasTextureCoords())
		{

		}
	}

	return ret;
}

void Mesh::loadToOpenGl()
{

}