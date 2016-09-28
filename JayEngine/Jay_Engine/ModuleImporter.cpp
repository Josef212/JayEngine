#include "Application.h"
#include "Globals.h"
#include "ModuleImporter.h"

//TMP
#include "OpenGLDraws.h"

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

bool ModuleImporter::cleanUp()
{
	bool ret = true;

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}

void ModuleImporter::loadFBX(const char* path)
{
	if (!path)
	{
		LOG("Error while loading fbx: path is NULL.");
		return; //If path is NULL dont do nothing
	}

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene && scene->HasMeshes())
	{
		meshes = new Mesh[scene->mNumMeshes];
		//Iterate all meshes in the scene
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh m;
			m.numVertices = scene->mMeshes[i]->mNumVertices;
			m.vertices = new float[m.numVertices * 3];
			memcpy(m.vertices, scene->mMeshes[i]->mVertices, sizeof(float)*m.numVertices * 3);
			LOG("New mesh with %d vertices", m.numVertices);

			if (scene->mMeshes[i]->mNumFaces * 3)
			{
				m.numIndices = scene->mMeshes[i]->mNumFaces * 3;
				m.indices = new uint[m.numIndices]; //Assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++i)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m.indices[i * 3], scene->mMeshes[i]->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

			meshes[i] = m;
			++meshesNum;
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
	}

}

void ModuleImporter::drawAllFBX()
{
	if (meshes)
	{
		for (int i = 0; i < meshesNum; ++i)
		{

		}
	}
}