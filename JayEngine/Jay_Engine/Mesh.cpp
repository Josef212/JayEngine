#include "Application.h"
#include "Mesh.h"

#include "ModuleResourceManager.h"
#include "ResourceMesh.h"
#include "OpenGL.h"

//TMP: will change assimp for importer in order to use my own format
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"


Mesh::Mesh(GameObject* gObj, int id) : Component(gObj, id)
{
	type = MESH;
	//idTexture = -1;
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
	clearMesh();
}

void Mesh::clearMesh()
{
	/*if (idIndices > 0) glDeleteBuffers(1, &idIndices);
	if (idVertices > 0) glDeleteBuffers(1, &idVertices);
	if (idNormals > 0) glDeleteBuffers(1, &idNormals);
	if (idTexCoords > 0) glDeleteBuffers(1, &idTexCoords);

	numIndices = 0;
	numVertices = 0;
	numNormals = 0;
	numTexCoords = 0;

	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(texCoords);*/
}

bool Mesh::loadMesh(aiMesh* mesh, bool loadToRAM)
{
	bool ret = false;

	meshResource = (ResourceMesh*)app->resourceManager->createNewResource(RESOURCE_MESH);

	if (mesh)
	{
		if(mesh->mName.length > 0)
			setName(mesh->mName.C_Str());
		
		meshResource->numVertices = mesh->mNumVertices;
		meshResource->vertices = new float[meshResource->numVertices * 3];
		memcpy(meshResource->vertices, mesh->mVertices, sizeof(float)*meshResource->numVertices * 3);
		_LOG(LOG_STD, "New mesh called %s with %d vertices.", getName(), meshResource->numVertices);

		if (mesh->HasFaces())
		{
			meshResource->numIndices = mesh->mNumFaces * 3;
			meshResource->indices = new uint[meshResource->numIndices]; //Assume each face is a triangle
			for (uint i = 0; i < mesh->mNumFaces; ++i)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
				{
					_LOG(LOG_WARN, "WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&meshResource->indices[i * 3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
				}
			}

			_LOG(LOG_STD, "Mesh %s has %d indices.", getName(), meshResource->numIndices);
		}

		if (mesh->HasNormals())
		{
			meshResource->numNormals = meshResource->numVertices * 3;
			meshResource->normals = new float[meshResource->numNormals];
			memcpy(meshResource->normals, mesh->mNormals, sizeof(float) * meshResource->numNormals);

			_LOG(LOG_STD, "Mesh %s has %d normal.", getName(), meshResource->numNormals / 3);
		}

		if (mesh->HasTextureCoords(0))//Difuse 
		{
			meshResource->numTexCoords = meshResource->numVertices * 2;
			meshResource->texCoords = new float[meshResource->numTexCoords];
			aiVector3D* tmp = mesh->mTextureCoords[0];
			for (int i = 0; i < meshResource->numTexCoords; i += 2)
			{
				meshResource->texCoords[i] = tmp->x;
				meshResource->texCoords[i + 1] = tmp->y;
				++tmp;
			}
			_LOG(LOG_STD, "Mesh %s has %d texture coords.", getName(), meshResource->numTexCoords / 2);
		}

		ret = true;

		if (loadToRAM)
		{
			if (onVRAM)
			{
				//Should remove current mesh in order to load new
				_LOG(LOG_WARN, "Mesh already loaded to VRAM");
			}
			ret = loadToOpenGl();
		}
	}

	return ret;
}

bool Mesh::loadToOpenGl()
{
	bool ret = false;

	if (onVRAM)
		return true;

	_LOG(LOG_STD, "Loading mesh to VRAM");

	if (meshResource->numVertices > 0 && meshResource->numIndices > 0)
	{
		glGenBuffers(1, (GLuint*)&meshResource->idVertices);
		glGenBuffers(1, (GLuint*)&meshResource->idIndices);

		glBindBuffer(GL_ARRAY_BUFFER, meshResource->idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshResource->numVertices * 3, meshResource->vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshResource->idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshResource->numIndices, meshResource->indices, GL_STATIC_DRAW);

		if (meshResource->numNormals > 0)
		{
			glGenBuffers(1, (GLuint*)&meshResource->idNormals);
			glBindBuffer(GL_ARRAY_BUFFER, meshResource->idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshResource->numNormals, meshResource->normals, GL_STATIC_DRAW);
		}
	
		if (meshResource->numTexCoords> 0)
		{
			glGenBuffers(1, (GLuint*)&meshResource->idTexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, meshResource->idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshResource->numTexCoords, meshResource->texCoords, GL_STATIC_DRAW);
		}

		ret = true;
	}
	else
	{
		_LOG(LOG_ERROR, "Could not load to VRAM because there are no vertices or indices. Vertices number: %d, indices number: %d", meshResource->numVertices, meshResource->numIndices);
		ret = false;
	}

	onVRAM = ret;

	return ret;
}

ResourceMesh* Mesh::createAnEmptyMeshRes()
{
	if (!meshResource)
	{
		meshResource = (ResourceMesh*)app->resourceManager->createNewResource(RESOURCE_MESH);
	}
	
	return meshResource;
}