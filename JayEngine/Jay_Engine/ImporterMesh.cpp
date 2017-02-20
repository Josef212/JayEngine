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

#include "OpenGL.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ImporterMesh::ImporterMesh() : Importer()
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

//------------------------------------------------------------

bool ImporterMesh::LoadResource(Resource* resource)
{
	bool ret = false;

	if (!resource || resource->GetType() != RESOURCE_MESH || resource->exportedFile.empty())
		return ret;

	ResourceMesh* res = (ResourceMesh*)resource;

	char* buffer = nullptr;
	uint size = app->fs->Load(res->GetExportedFileStr(), PATH_LIBRARY_MESH, &buffer);

	if (buffer && size > 0)
	{
		char* cursor = buffer;

		//Ranges
		uint ranges[5];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		res->numIndices = ranges[0];
		res->numVertices = ranges[1];

		//Indices
		cursor += bytes;
		bytes = sizeof(uint) * res->numIndices;
		res->indices = new uint[res->numIndices];
		memcpy(res->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(float) * res->numVertices * 3;
		res->vertices = new float[res->numVertices * 3];
		memcpy(res->vertices, cursor, bytes);

		//Colors
		if (ranges[2] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 3;
			res->colors = new float[res->numVertices * 3];
			memcpy(res->colors, cursor, bytes);
		}

		//Normals
		if (ranges[3] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 3;
			res->normals = new float[res->numVertices * 3];
			memcpy(res->normals, cursor, bytes);
		}

		//Texture Coords
		if (ranges[4] > 0)
		{
			cursor += bytes;
			bytes = sizeof(float) * res->numVertices * 2;
			res->texCoords = new float[res->numVertices * 2];
			memcpy(res->texCoords, cursor, bytes);
		}

		//AABB
		cursor += bytes;
		bytes = sizeof(AABB);
		memcpy(&res->aabb, cursor, bytes);

		ret = true;
	}

	RELEASE_ARRAY(buffer);

	GenBuffers(res);

	return ret;
}

bool ImporterMesh::Import(const aiMesh* mesh, std::string& output, UID& id)
{
	bool ret = false;

	if (!mesh)
		return ret;

	//Create a tmp resource to allocate all info.
	ResourceMesh m(0);


	//Vertices
	m.numVertices = mesh->mNumVertices;
	m.vertices = new float[m.numVertices * 3];
	memcpy(m.vertices, mesh->mVertices, sizeof(float) * m.numVertices * 3);

	//Faces - indices
	if (mesh->HasFaces())
	{
		m.numIndices = mesh->mNumFaces * 3;
		m.indices = new uint[m.numIndices];
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
				_LOG(LOG_WARN, "WARNING, geometry face with != 3 indices!");
			
			memcpy(&m.indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	//Normals
	if (mesh->HasNormals())
	{
		m.normals = new float[m.numVertices * 3];
		memcpy(m.normals, mesh->mNormals, sizeof(float) * m.numVertices * 3);
	}

	//Colors
	if (mesh->HasVertexColors(0))
	{
		m.colors = new float[m.numVertices * 3];
		memcpy(m.colors, mesh->mColors, sizeof(float) * m.numVertices * 3);
	}

	//TextureCoords
	/*if (mesh->HasTextureCoords(0))			//If using 3D textures
	{
		m.texCoords = new float[m.numVertices * 3];
		memcpy(m.texCoords, mesh->mTextureCoords, sizeof(float) * m.numVertices * 3);
	}*/

	if (mesh->HasTextureCoords(0))
	{
		m.texCoords = new float[m.numVertices * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (uint i = 0; i < m.numVertices * 2; i += 2)
		{
			m.texCoords[i] = tmp->x;
			m.texCoords[i + 1] = tmp->y;
			++tmp;
		}
	}

	//Enclosing box
	m.aabb.SetNegativeInfinity();
	m.aabb.Enclose((float3*)m.vertices, m.numVertices);

	//Now save the mesh into a binary file.

	uint ranges[5] = {
		m.numIndices,
		m.numVertices,
		(m.colors) ? m.numVertices : 0,
		(m.normals) ? m.numVertices : 0,
		(m.texCoords) ? m.numVertices : 0
	};

	uint size = sizeof(ranges) + sizeof(uint) * m.numIndices
		+ sizeof(float) * m.numVertices * 3;
	if (m.colors)sizeof(float) * m.numVertices * 3;
	if (m.normals) size += sizeof(float) * m.numVertices * 3;
	if (m.texCoords)sizeof(float) * m.numVertices * 3;
	size += sizeof(AABB);

	//Allocate memory
	char* data = new char[size];
	char* cursor = data;

	//Ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	//Indices
	cursor += bytes;
	bytes = sizeof(uint) * m.numIndices;
	memcpy(cursor, m.indices, bytes);

	//Vertices
	cursor += bytes;
	bytes = sizeof(float) * m.numVertices * 3;
	memcpy(cursor, m.vertices, bytes);

	//Colors
	cursor += bytes;
	bytes = sizeof(float) * m.numVertices * 3;
	memcpy(cursor, m.colors, bytes);

	//Normals
	cursor += bytes;
	bytes = sizeof(float) * m.numVertices * 3;
	memcpy(cursor, m.normals, bytes);

	//Texture Coords
	cursor += bytes;
	bytes = sizeof(float) * m.numVertices * 2;
	memcpy(cursor, m.texCoords, bytes);

	//AABB
	cursor += bytes;
	bytes = sizeof(AABB);
	memcpy(cursor, &m.aabb, bytes);

	//Ready to save

	id = app->resourceManager->GetNewUID();
	output.assign(PATH_LIBRARY_MESH + std::to_string(id) + EXTENSION_MESH);

	if (app->fs->Save(output.c_str(), data, size) != size)
	{
		_LOG(LOG_ERROR, "ERROR: Saving a mesh: %s.", output.c_str());
	}
	else
	{
		ret = true;
	}

	RELEASE_ARRAY(data);

	return ret;
}

void ImporterMesh::GenBuffers(const ResourceMesh* resource)
{
	if (!resource)
		return;

	if (resource->indices && resource->vertices)
	{
		//Vertices
		glGenBuffers(1, (GLuint*)&resource->idVertices);
		glBindBuffer(GL_ARRAY_BUFFER, resource->idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * resource->numVertices * 3, resource->vertices, GL_STATIC_DRAW);

		//Indices
		glGenBuffers(1, (GLuint*)&resource->idIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource->idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * resource->numIndices, resource->indices, GL_STATIC_DRAW);

		//Colors
		if (resource->colors)
		{
			glGenBuffers(1, (GLuint*)&resource->idColors);
			glBindBuffer(GL_ARRAY_BUFFER, resource->idColors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * resource->numVertices * 3, resource->colors, GL_STATIC_DRAW);
		}

		//Normals
		if (resource->normals)
		{
			glGenBuffers(1, (GLuint*)&resource->idNormals);
			glBindBuffer(GL_ARRAY_BUFFER, resource->idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * resource->numVertices * 3, resource->normals, GL_STATIC_DRAW);
		}

		//Texture Coords
		if (resource->texCoords)
		{
			glGenBuffers(1, (GLuint*)&resource->idTexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, resource->idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * resource->numVertices * 2, resource->texCoords, GL_STATIC_DRAW);
		}

		//TODO: Adapt this to use shaders

	}
}

//------------------------------------------------------------

bool ImporterMesh::LoadCube(ResourceMesh* res)
{
	if (!res)
		return false;

	res->originalFile = "-Cube-";
	res->exportedFile = "-Cube-";

	//Indices
	uint indices[6 * 6] = 
	{
		2, 7, 6, 2, 3, 7, // front
		11, 9, 10, 11, 8, 9, // right
		1, 4, 0, 1, 5, 4, // back
		15, 13, 12, 15, 14, 13, // left
		1, 3, 2, 1, 0, 3, // top
		7, 5, 6, 7, 4, 5  // bottom
	};

	res->numIndices = 36;
	uint bytes = sizeof(uint) * res->numIndices;
	res->indices = new uint[res->numIndices];
	memcpy(res->indices, indices, bytes);

	//Vertices
	float vertices[16 * 3] =
	{
		0.5f,  0.5f,  0.5f, // 0
		-0.5f,  0.5f,  0.5f, // 1
		-0.5f,  0.5f, -0.5f, // 2
		0.5f,  0.5f, -0.5f, // 3

		0.5f, -0.5f,  0.5f, // 4
		-0.5f, -0.5f,  0.5f, // 5
		-0.5f, -0.5f, -0.5f, // 6
		0.5f, -0.5f, -0.5f, // 7

		// add repeated vertices for proper texturing
		0.5f,  0.5f,  0.5f,  // 8
		0.5f, -0.5f,  0.5f,  // 9
		0.5f, -0.5f, -0.5f,  //10
		0.5f,  0.5f, -0.5f,  //11

		-0.5f,  0.5f,  0.5f,  //12
		-0.5f, -0.5f,  0.5f,  //13
		-0.5f, -0.5f, -0.5f,  //14
		-0.5f,  0.5f, -0.5f,  //15
	};

	res->numVertices = 16;
	bytes = sizeof(float) * res->numVertices * 3;
	res->vertices = new float[res->numVertices * 3];
	memcpy(res->vertices, vertices, bytes);

	//Texture coords
	float textureCoords[16 * 3] =
	{
		1.f,  1.f,  0.f,
		0.f,  1.f,  0.f,
		0.f,  0.f,  0.f,
		1.f,  0.f,  0.f,

		1.f,  0.f,  0.f,
		0.f,  0.f,  0.f,
		0.f,  1.f,  0.f,
		1.f,  1.f,  0.f,

		// extra coords for left - right
		1.f,  1.f,  0.f,
		0.f,  1.f,  0.f,
		0.f,  0.f,  0.f,
		1.f,  0.f,  0.f,

		0.f,  1.f,  0.f,
		1.f,  1.f,  0.f,
		1.f,  0.f,  0.f,
		0.f,  0.f,  0.f,
	};

	res->texCoords = new float[res->numVertices * 3];
	memcpy(res->texCoords, textureCoords, bytes);

	//Normals
	//TODO

	res->aabb = AABB(float3(-0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f));

	GenBuffers(res);

	return true;
}

bool ImporterMesh::LoadSphere(ResourceMesh* res)
{
	bool ret = false;

	return ret;
}

bool ImporterMesh::LoadCylinder(ResourceMesh* res)
{
	bool ret = false;

	return ret;
}

bool ImporterMesh::LoadCone(ResourceMesh* res)
{
	bool ret = false;

	return ret;
}

bool ImporterMesh::LoadPyramid(ResourceMesh* res)
{
	bool ret = false;

	return ret;
}

bool ImporterMesh::LoadTorus(ResourceMesh* res)
{
	bool ret = false;

	return ret;
}

//------------------------------------------------------------
