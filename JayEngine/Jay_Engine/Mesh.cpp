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
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(texCoords);
}

bool Mesh::loadMesh(aiMesh* mesh, bool loadToRAM)
{
	bool ret = false;

	if (mesh)
	{
		if(mesh->mName.length > 0)
			setName(mesh->mName.C_Str());
		
		numVertices = mesh->mNumVertices;
		vertices = new float[numVertices * 3];
		memcpy(vertices, mesh->mVertices, sizeof(float)*numVertices * 3);
		_LOG("New mesh called %s with %d vertices.", getName(), numVertices);

		if (mesh->HasFaces())
		{
			numIndices = mesh->mNumFaces * 3;
			indices = new uint[numIndices]; //Assume each face is a triangle
			for (uint i = 0; i < mesh->mNumFaces; ++i)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
				{
					_LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
				}
			}
			_LOG("Mesh %s has %d indices.", getName(), numIndices);
		}

		if (mesh->HasNormals())
		{
			numNormals = numVertices * 3;
			normals = new float[numNormals];
			memcpy(normals, mesh->mNormals, sizeof(float) * numNormals);

			_LOG("Mesh %s has %d normal.", getName(), numNormals / 3);
		}

		if (mesh->HasTextureCoords(0))
		{
			numTexCoords = numVertices * 2;
			texCoords = new float[numTexCoords];
			for (uint i = 0; i < numTexCoords; ++i)
			{
				memcpy(&texCoords[i * 2], &mesh->mTextureCoords[0][i*2+i], sizeof(float) * numTexCoords);
			}
			_LOG("Mesh %s has %d texture coords.", getName(), numTexCoords / 2);
		}

		ret = true;

		if (loadToRAM)
		{
			if (onVRAM)
			{
				//Should remove current mesh in order to load new
				_LOG("Mesh already loaded to VRAM");
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

	_LOG("Loading mesh to VRAM");

	if (numVertices > 0 && numIndices > 0)
	{
		//_LOG("1.Creating indices and vertices buffers");
		glGenBuffers(1, (GLuint*)&idVertices);
		glGenBuffers(1, (GLuint*)&idIndices);
		_LOG("Buffs generated");
		glBindBuffer(GL_ARRAY_BUFFER, idVertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertices, GL_STATIC_DRAW);
		_LOG("Vertices");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numIndices * 3, indices, GL_STATIC_DRAW);
		_LOG("Indices");
		_LOG("Loaded vertices and indices");
		if (numNormals > 0)
		{
			glGenBuffers(1, (GLuint*)&idNormals);
			glBindBuffer(GL_ARRAY_BUFFER, idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numNormals, normals, GL_STATIC_DRAW);
			_LOG("Loaded normals");
		}
	
		if (numTexCoords> 0)
		{
			glGenBuffers(1, (GLuint*)&idTexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numTexCoords, texCoords, GL_STATIC_DRAW);
			_LOG("Loaded UV's");
		}

		ret = true;
	}
	else
	{
		_LOG("Could not load to VRAM because there are no vertices or indices. Vertices number: %d, indices number: %d", numVertices, numIndices);
		ret = false;
	}

	onVRAM = ret;

	return ret;
}