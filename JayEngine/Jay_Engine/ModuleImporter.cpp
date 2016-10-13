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
	name.assign("module_importer");
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
	//aiDetachAllLogStreams();

	return ret;
}

void ModuleImporter::loadFBX(const char* path, std::vector<VertexInfo>& vec)
{
	if (!path)
	{
		_LOG("Error while loading fbx: path is NULL.");
		return; //If path is NULL dont do nothing
	}

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->HasMeshes())
	{
		//Iterate all meshes in the scene
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			VertexInfo m;
			m.numVertices = scene->mMeshes[i]->mNumVertices;
			m.vertices = new float[m.numVertices * 3];
			memcpy(m.vertices, scene->mMeshes[i]->mVertices, sizeof(float)*m.numVertices * 3);
			_LOG("New mesh with %d vertices", m.numVertices);

			if (scene->mMeshes[i]->HasFaces())
			{
				m.numIndices = scene->mMeshes[i]->mNumFaces * 3;
				m.indices = new uint[m.numIndices]; //Assume each face is a triangle
				m.numNormals = m.numVertices * 3;
				m.normals = new float[m.numNormals];

				if (scene->mMeshes[i]->HasNormals())
				{
					memcpy(m.normals, scene->mMeshes[i]->mNormals, sizeof(float) * m.numNormals);
				}

				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						_LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m.indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
				glGenBuffers(1, (GLuint*)&m.idVertices);
				glBindBuffer(GL_ARRAY_BUFFER, m.idVertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.numVertices * 3, m.vertices, GL_STATIC_DRAW); //Care: mult numVertices per 3

				glGenBuffers(1, (GLuint*)&m.idNormals);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.idNormals);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.numNormals, m.normals, GL_STATIC_DRAW);

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
		_LOG("Error loading scene %s", path);
	}
}

void ModuleImporter::drawMeshes(std::vector<VertexInfo> vec)
{
	for (uint i = 0; i < vec.size(); ++i)
	{
		VertexInfo* m = &vec.at(i);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idNormals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);
		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}