#include "Application.h"
#include "ImporterMesh.h"

#include "ResourceMesh.h"
#include "ModuleFileSystem.h"
#include "FileParser.h"
#include "ModuleResourceManager.h"
#include <string>

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ImporterMesh::ImporterMesh()
{
	_LOG(LOG_STD, "Creating a mesh importer.");

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


ImporterMesh::~ImporterMesh()
{
	//Stop log stream
	aiDetachAllLogStreams();
}

void ImporterMesh::importMesh(const aiMesh* mesh, ResourceMesh* resMesh)
{
	if (!mesh || !resMesh)
	{
		_LOG(LOG_ERROR, "Invalid aiMesh or resource mesh.");
		return;
	}

	//First set the exported file name of the resource from its uuid and own extension
	std::string outName(DEFAULT_MESH_SAVE_PATH + std::to_string(resMesh->getUID()) + MESH_EXTENSION);
	//Will also put the uuid at the start of the file in order to not get the uuid from the file name

	//TODO: set origin file
	resMesh->exportedFile.assign(std::to_string(resMesh->getUID()) + MESH_EXTENSION);

	_LOG(LOG_INFO_REM, "New mesh is going to be serialized: %s.", outName.c_str());

#pragma region Filling mesh resource
	//----------------------------------------------
	if (mesh->HasFaces())
	{
		resMesh->numIndices = mesh->mNumFaces * 3;
		resMesh->indices = new uint[resMesh->numIndices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				_LOG(LOG_WARN, "WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&resMesh->indices[i * 3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
			}
		}
		//_LOG(LOG_STD, "Mesh has %d indices.", resMesh->numIndices);
	}
	//----------------------------------------------

	resMesh->numVertices = mesh->mNumVertices;
	resMesh->vertices = new float[resMesh->numVertices * 3];
	memcpy(resMesh->vertices, mesh->mVertices, sizeof(float) * resMesh->numVertices * 3);

	//_LOG(LOG_STD, "Mesh has %d vertices.", resMesh->numVertices);

	//----------------------------------------------

	if (mesh->HasNormals())
	{
		resMesh->numNormals = resMesh->numVertices;
		resMesh->normals = new float[resMesh->numNormals * 3];
		memcpy(resMesh->normals, mesh->mNormals, sizeof(float) * resMesh->numNormals * 3);

		//_LOG(LOG_STD, "Mesh has %d normals.", resMesh->numNormals);
	}

	//----------------------------------------------

	if (mesh->HasTextureCoords(0))
	{
		resMesh->numTexCoords = resMesh->numVertices;
		resMesh->texCoords = new float[resMesh->numTexCoords * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (uint i = 0; i < resMesh->numTexCoords * 2; i += 2)
		{
			resMesh->texCoords[i] = tmp->x;
			resMesh->texCoords[i + 1] = tmp->y;
			++tmp;
		}

		//_LOG(LOG_STD, "Mesh has %d UV's.", resMesh->numTexCoords);
	}

	resMesh->aabb.SetNegativeInfinity();
	resMesh->aabb.Enclose((float3*)resMesh->vertices, resMesh->numVertices);

	//----------------------------------------------
#pragma endregion

	//Now we have the resource filled lets create the file to store it

	uint ranges[5] = {
		resMesh->numIndices,
		resMesh->numVertices,
		(resMesh->normals) ? resMesh->numNormals : 0,
		(resMesh->texCoords) ? resMesh->numTexCoords : 0,
		0 //TODO: Colors
	};

	uint size = sizeof(ranges) + sizeof(uint) * resMesh->numIndices + sizeof(float) * resMesh->numVertices * 3;
	if (resMesh->normals) size += sizeof(float) * resMesh->numNormals * 3;
	if (resMesh->texCoords) size += sizeof(float) * resMesh->numTexCoords * 2;
	//TODO: Colors
	size += sizeof(AABB);

	//Allocate memory
	char* data = new char[size];
	char* cursor = data;

	//First store ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	//Second store indices
	cursor += bytes;
	bytes = sizeof(uint) * resMesh->numIndices;
	memcpy(cursor, resMesh->indices, bytes);

	//Third store vertices
	cursor += bytes;
	bytes = sizeof(float) * resMesh->numVertices * 3;
	memcpy(cursor, resMesh->vertices, bytes);

	//Fourth store normals
	if (resMesh->normals)
	{
		cursor += bytes;
		bytes = sizeof(float) * resMesh->numNormals * 3;
		memcpy(cursor, resMesh->normals, bytes);
	}

	//Fifth store uv's
	if (resMesh->texCoords)
	{
		cursor += bytes;
		bytes = sizeof(float) * resMesh->numTexCoords * 2;
		memcpy(cursor, resMesh->texCoords, bytes);
	}

	//Sixth store colors //TODO

	//Seventh store AABB
	cursor += bytes;
	bytes = sizeof(AABB);
	memcpy(cursor, &resMesh->aabb.minPoint.x, bytes);

	if (app->fs->save(outName.c_str(), data, size) != size)
		_LOG(LOG_ERROR, "ERROR saving the mesh.");

	RELEASE_ARRAY(data);
}
