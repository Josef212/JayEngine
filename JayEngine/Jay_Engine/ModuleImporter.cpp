#include "Application.h"
#include "Globals.h"
#include "ModuleImporter.h"

//TMP
//#include "OpenGLDraws.h"
#include "OpenGL.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(bool startEnabled) : Module(startEnabled)
{
}


ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::init()
{
	bool ret = true;

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleImporter::start()
{
	//loadFBX("Assets/fbx/warrior.FBX", meshes);
	//loadFBX("Assets/fbx/Brute.fbx", meshes);

	return true;
}

update_status ModuleImporter::postUpdate(float dt)
{	
	//drawMeshes(meshes);

	return UPDATE_CONTINUE;
}

bool ModuleImporter::cleanUp()
{
	bool ret = true;

	meshes.clear();

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}

void ModuleImporter::loadFBX(const char* path, std::vector<Mesh>& vec)
{
	if (!path)
	{
		LOG("Error while loading fbx: path is NULL.");
		return; //If path is NULL dont do nothing
	}

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->HasMeshes())
	{
		//Iterate all meshes in the scene
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh m;
			m.numVertices = scene->mMeshes[i]->mNumVertices;
			m.vertices = new float[m.numVertices * 3];
			memcpy(m.vertices, scene->mMeshes[i]->mVertices, sizeof(float)*m.numVertices * 3);
			LOG("New mesh with %d vertices", m.numVertices);

			if (scene->mMeshes[i]->HasFaces())
			{
				m.numIndices = scene->mMeshes[i]->mNumFaces * 3;
				m.indices = new uint[m.numIndices]; //Assume each face is a triangle
				m.numFaces = scene->mMeshes[i]->mNumFaces;
				m.normals = new float[m.numNormals];

				if (scene->mMeshes[i]->HasNormals())
				{
					memcpy(m.normals, scene->mMeshes[i]->mNormals, sizeof(float) * m.numFaces * 3);
				}

				for (uint j = 0; j < m.numFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m.indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				glGenBuffers(1, (GLuint*)&m.idVertices);
				glBindBuffer(GL_ARRAY_BUFFER, m.idVertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.numVertices, m.vertices, GL_STATIC_DRAW);

				glGenBuffers(1, (GLuint*)&m.idIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.idIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.numIndices, m.indices, GL_STATIC_DRAW);
			}

			vec.push_back(m);
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}
}

void ModuleImporter::drawMeshes(std::vector<Mesh> vec)
{
	for (uint i = 0; i < vec.size(); ++i)
	{
		Mesh* m = &vec.at(i);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
}