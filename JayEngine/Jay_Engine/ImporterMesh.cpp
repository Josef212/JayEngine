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

void ImporterMesh::importFBX(const char* fbxName)
{
	if (!fbxName)
	{
		_LOG(LOG_ERROR, "Could not load the fbx: Invalid fbx name or output name.");
		return;
	}

	std::string realFBXPath(DEFAULT_FB_PATH);
	realFBXPath.append("/");
	realFBXPath.append(fbxName);
	//TODO: clear fbx name file here or the parameter is already cleaned
	//For now will add the Assets/fbx path here

	_LOG(LOG_INFO, "Loading fbx: %s.", realFBXPath.c_str());

	char* buffer;
	uint fileSize = app->fs->load(realFBXPath.c_str(), &buffer);
	const aiScene* scene = NULL;
	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		_LOG(LOG_ERROR, "Error while loading fbx: %s.", realFBXPath.c_str());
		return;
	}

	//TODO: I want to build the game object hierarchy here and generate the json file without adding to the scene
	//For now will serialitzate only meshes directly.
	//Maybe will put this method code to Resource manager file and let only mesh import here

	if (scene && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			ResourceMesh* resMesh = (ResourceMesh*)app->resourceManager->createNewResource(RESOURCE_MESH);
			if (resMesh)
			{
				resMesh->originalFile.assign(realFBXPath);
				importMesh(scene->mMeshes[i], resMesh);
			}
			else
				_LOG(LOG_ERROR, "Error: Could not generate a new mesh resource.");
		}

		aiReleaseImport(scene);
	}

	RELEASE_ARRAY(buffer);
}

void ImporterMesh::importMesh(aiMesh* mesh, ResourceMesh* resMesh)
{
	if (!mesh || !resMesh)
	{
		_LOG(LOG_ERROR, "Invalid aiMesh or resource mesh.");
		return;
	}

	//First set the exported file name of the resource from its uuid and own extension
	std::string outName(DEFAULT_MESH_SAVE_PATH + std::to_string(resMesh->getUID()) + MESH_EXTENSION);
	//Will also put the uuid at the start of the file in order to not get the uuid from the file name

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


	//Sixth stroe colors //TODO

	if (app->fs->save(outName.c_str(), data, size) != size)
		_LOG(LOG_ERROR, "ERROR saving the mesh.");

	RELEASE_ARRAY(data);
}

void ImporterMesh::loadMesh(const char* fileName, ResourceMesh* resMesh)
{
	if (!fileName || !resMesh)
	{
		_LOG(LOG_ERROR, "Error loading a mesh .jof: Invalid file name or mesh resource.");
		return;
	}

	std::string realName(DEFAULT_MESH_SAVE_PATH);
	realName.append(fileName);

	_LOG(LOG_INFO, "Loading mesh: %s.", realName.c_str());

	char* data;
	uint size = app->fs->load(realName.c_str(), &data);

	if (data && size > 0)
	{
		uint ranges[5];
		char* cursor = data;
		uint bytes = sizeof(ranges);

		//Ranges
		memcpy(ranges, cursor, bytes);

		resMesh->numIndices = ranges[0];
		resMesh->numVertices = ranges[1];
		resMesh->numNormals = ranges[2];
		resMesh->numTexCoords = ranges[3];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * resMesh->numIndices;

		resMesh->indices = new uint[resMesh->numIndices];
		memcpy(resMesh->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * resMesh->numVertices * 3;

		resMesh->vertices = new float[resMesh->numVertices * 3];
		memcpy(resMesh->vertices, cursor, bytes);

		//Normals
		if (ranges[2] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * resMesh->numNormals * 3;

			resMesh->normals = new float[resMesh->numNormals * 3];
			memcpy(resMesh->normals, cursor, bytes);
		}

		//UV's
		if (ranges[3])
		{
			cursor += bytes;
			bytes = sizeof(float) * resMesh->numTexCoords * 2;

			resMesh->texCoords = new float[resMesh->numTexCoords * 2];
			memcpy(resMesh->texCoords, cursor, bytes);
		}

	}
	else
		_LOG(LOG_ERROR, "Error loading the mesh: '%s'.", realName.c_str());


	RELEASE_ARRAY(data);
}