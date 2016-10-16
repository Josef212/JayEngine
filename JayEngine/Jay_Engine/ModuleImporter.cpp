#include "Application.h"
#include "Globals.h"
#include "ModuleImporter.h"

//TMP
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












void ModuleImporter::loadFBXs(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// use scene->mNumMeshes to iterate on scene->mMeshes array
		//meshes = new VramVertex[scene->mNumMeshes];

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Todo
			VramVertex* mesh = new VramVertex();
			aiMesh* new_mesh = scene->mMeshes[i];

			mesh->numVertices = new_mesh->mNumVertices;
			mesh->vertices = new float[mesh->numVertices * 3];

			memcpy(mesh->vertices, new_mesh->mVertices, sizeof(float)*  mesh->numVertices * 3);

			//Normals
			if (new_mesh->HasNormals())
			{
				mesh->numNormals = new_mesh->mNumVertices;
				mesh->normals = new float[mesh->numNormals * 3];

				memcpy(mesh->normals, new_mesh->mNormals, sizeof(float) * mesh->numNormals * 3);
			}

			if (new_mesh->HasFaces())
			{
				SDL_Log("New mesh with %d vertices", mesh->numVertices);

				if (scene->mMeshes[i]->mNumFaces * 3)
				{
					mesh->numIndices = scene->mMeshes[i]->mNumFaces * 3;
					mesh->indices = new uint[mesh->numIndices]; // Asume all are triangles
					for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
					{
						if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
						{
							SDL_Log("WARNING, geometry face with != 3 indices!");
						}
						else
						{
							memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
						}
					}
				}
			}

			//UV
			//if (new_mesh->HasTextureCoords(0))
			//{
			//	mesh->UV = new float[mesh->numVertices * 3];
			//	for (uint i = 0; i < mesh->numVertices*2; i++)
			//	{
			//		memcpy(&mesh->UV[i*2], &new_mesh->mTextureCoords[0][i*2+i], sizeof(float) * mesh->numVertices * 2);
			//	}				
			//}
			if (new_mesh->HasTextureCoords(0))
			{
				mesh->UV = new float[mesh->numVertices * 3];
				memcpy(mesh->UV, new_mesh->mTextureCoords[0], sizeof(float) * mesh->numVertices * 3);
			}

			Mmeshes.push_back(mesh);

			//Generating GL Buffers

			//vertices
			glGenBuffers(1, (GLuint*) &(Mmeshes[i]->idVertices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mmeshes[i]->idVertices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*Mmeshes[i]->numVertices * 3, Mmeshes[i]->vertices, GL_STATIC_DRAW);

			//normals
			if (mesh->normals != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(Mmeshes[i]->idNormals));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mmeshes[i]->idNormals);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*Mmeshes[i]->numNormals * 3, Mmeshes[i]->normals, GL_STATIC_DRAW);
			}
			//indices
			glGenBuffers(1, (GLuint*) &(Mmeshes[i]->idIndices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mmeshes[i]->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*Mmeshes[i]->numIndices, Mmeshes[i]->indices, GL_STATIC_DRAW);

			//UV
			if (mesh->UV != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(Mmeshes[i]->idUV));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mmeshes[i]->idUV);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*Mmeshes[i]->numVertices * 3, Mmeshes[i]->UV, GL_STATIC_DRAW);
			}

		}
		aiReleaseImport(scene);
	}
	else
		SDL_Log("Error loading Scene %s", full_path);

}

void ModuleImporter::drawMeshes(std::vector<VramVertex*> drawMeshes)
{
	for (uint i = 0; i < drawMeshes.size(); i++)
	{
		VramVertex* m = drawMeshes.at(i);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//vertices
		glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		//normals
		glBindBuffer(GL_ARRAY_BUFFER, m->idNormals);
		glNormalPointer(GL_FLOAT, 0, NULL);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);

		//UV
		glBindBuffer(GL_ARRAY_BUFFER, m->idUV);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);


		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}
}