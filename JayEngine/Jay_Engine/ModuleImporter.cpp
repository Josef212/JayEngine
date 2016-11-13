#include "Application.h"
#include "Globals.h"
#include "ModuleImporter.h"

#include "ModuleFileSystem.h"

#include <string>

#include "Mesh.h"
//TMP
#include "Timer.h"
#include "ModuleManager.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


ModuleImporter::ModuleImporter(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Importer: Creation.");
	name.assign("module_importer");
}


ModuleImporter::~ModuleImporter()
{
	_LOG(LOG_STD, "Importer: Destroying.");
}

bool ModuleImporter::init(FileParser* conf)
{
	_LOG(LOG_STD, "Importer: Init.");
	bool ret = true;

	//Log assimp info
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool ModuleImporter::start()
{
	_LOG(LOG_STD, "Importer: Start.");

	Timer timer;
	timer.Start();
	importFBX("Cube.fbx");
	//importFBX("Street environment_V01.FBX");
	//importFBX("MechaT.fbx");
	_LOG(LOG_STD, "Import fbx lasted: %dms.", timer.Read());
	timer.Stop();

	/*Timer timer2;
	timer2.Start();
	app->manager->loadFBX("Cube.fbx", NULL);
	_LOG(LOG_STD, "Import fbx lasted: %dms.", timer2.Read());

	Timer timer3;
	timer3.Start();
	loadMesh("cube.jof");
	_LOG(LOG_STD, "Reading the file lasted: %dms.", timer3.Read());*/

	return true;
}

bool ModuleImporter::cleanUp()
{
	_LOG(LOG_STD, "Importer: Init.");
	bool ret = true;

	//Stop log stream
	aiDetachAllLogStreams();

	return ret;
}

void ModuleImporter::importFBX(const char* fileFBX, const char* pathFBX)
{
	if (!fileFBX)
	{
		_LOG(LOG_ERROR, "Error: Invalid file name.");
		return;
	}

	char* realPath = new char[64];
	if (pathFBX)
		strcpy_s(realPath, 64, pathFBX);
	else
		strcpy_s(realPath, 64, DEFAULT_FB_PATH);

	sprintf_s(realPath, 64, "%s/%s", realPath, fileFBX);

	_LOG(LOG_WARN, "Importing fbx from: '%s'.", realPath);

	char* buffer;
	uint fileSize = app->fs->load(realPath, &buffer);

	const aiScene* scene = NULL;
	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		_LOG(LOG_ERROR, "Error while loading fbx.");
		return;
	}

	if (scene)
	{
		//_LOG(LOG_WARN, "Fbx loaded, num of meshes: %d.", scene->mNumMeshes);
		//_LOG(LOG_WARN, "Fbx loaded, num of materials: %d.", scene->mNumMaterials);
		//_LOG(LOG_WARN, "Fbx loaded, num of textures: %d.", scene->mNumTextures);
		//_LOG(LOG_STD, "---------------------------------------");

		//First will take the first mesh //TMP
		//--------------------------

		if (scene->HasMeshes())
		{
			//TODO: do this for each mesh
			aiMesh* mesh = scene->mMeshes[0];
			if (mesh->mNumVertices > 0 && mesh->mNumFaces > 0)
			{
				uint ranges[5] = {
					//1 the num of indices
					mesh->mNumFaces * 3,
					//2 the num of vertices
					mesh->mNumVertices,
					//3 the num of normals
					(mesh->HasNormals()) ? mesh->mNumVertices : 0,
					//4 the num of texture coords, difuse only for now
					(mesh->HasTextureCoords(0)) ? mesh->mNumVertices : 0,
					//5 the num of colors
					(mesh->HasVertexColors(0)) ? mesh->mNumVertices : 0
				};

				//_LOG(LOG_WARN, "Num of indices: %d", ranges[0]);
				//_LOG(LOG_WARN, "Num of vertices: %d", ranges[1]);
				//_LOG(LOG_WARN, "Num of normals: %d", ranges[2]);
				//_LOG(LOG_WARN, "Num of texCoords: %d", ranges[3]);
				//_LOG(LOG_WARN, "Num of VertexColors: %d", ranges[4]);
				//_LOG(LOG_STD, "---------------------------------------");

				uint size = sizeof(ranges) + sizeof(uint) * ranges[0] + sizeof(float) * ranges[1] * 3;
				if (ranges[2] > 0) size += sizeof(float) * ranges[2] * 3;
				if (ranges[3] > 0) size += sizeof(float) * ranges[3] * 2;
				if (ranges[4] > 0) size += sizeof(float) * ranges[4] * 3;

				//_LOG(LOG_WARN, "Buffer size: %d.", size);
				//_LOG(LOG_STD, "---------------------------------------");


				char* data = new char[size];
				char* cursor = data; //Just an iterator to move arround data.
				uint bytes = sizeof(ranges); //A tmp variable to know the amount of memory to copy every time.

				//Ranges
				memcpy(cursor, ranges, bytes);

				//Indices
				cursor += bytes;
				bytes = sizeof(ranges[0]);

				//NOTE: dont really like this
				if (mesh->HasFaces())
				{
					uint* indices = new uint[ranges[0]]; //Assume each face is a triangle
					for (uint i = 0; i < mesh->mNumFaces; ++i)
					{
						if (mesh->mFaces[i].mNumIndices != 3)
						{
							_LOG(LOG_WARN, "WARNING, geometry face with != 3 indices!");
						}
						else
						{
							memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, sizeof(uint) * 3);
						}
					}
					memcpy(cursor, indices, bytes);
					RELEASE_ARRAY(indices);
				}


				//Vertices
				cursor += bytes;
				bytes = sizeof(ranges[1]) * 3;
				memcpy(cursor, mesh->mVertices, bytes);

				//Normals
				if (mesh->HasNormals())
				{
					cursor += bytes;
					bytes = sizeof(ranges[2]) * 3;
					memcpy(cursor, mesh->mNormals, bytes);
				}

				//TexCoords
				if (mesh->HasTextureCoords(0))//Difuse 
				{
					cursor += bytes;
					bytes = sizeof(ranges[3]) * 2;

					float* texCoords = new float[bytes];
					aiVector3D* tmp = mesh->mTextureCoords[0];
					for (int i = 0; i < bytes; i += 2)
					{
						texCoords[i] = tmp->x;
						texCoords[i + 1] = tmp->y;
						++tmp;
					}
					memcpy(cursor, texCoords, bytes);
					RELEASE_ARRAY(texCoords);
				}

				//Colors
				if(mesh->HasVertexColors(0))
				{
					cursor += bytes;
					bytes = sizeof(ranges[4]) * 3;

					memcpy(cursor, mesh->mColors, bytes);
				}

				//const char* fileName = "cube.jof"; // = mesh->mName.C_Str();
				std::string saveName(DEFAULT_MESH_SAVE_PATH);
				saveName.append("cube.jof");
				
				if (app->fs->save(saveName.c_str(), data, size) != size)
				{
					_LOG(LOG_ERROR, "ERROR saving the mesh.");
				}


				RELEASE_ARRAY(data);

			}
			else
			{
				_LOG(LOG_ERROR, "Error: No vertices or indices. Num vertices: %d. Num indices: %d.", scene->mMeshes[0]->mNumVertices, scene->mMeshes[0]->mNumFaces * 3);
			}
		}


		//--------------------------
	}
	else
		_LOG(LOG_ERROR, "Error while loading the fbx. Assimp error.");


	aiReleaseImport(scene);

	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(realPath);
}

void ModuleImporter::loadMesh(const char* file, const char* path)
{
	if (!file)
	{
		_LOG(LOG_ERROR, "Error: Invalid file name.");
		return;
	}

	char* realPath = new char[64];
	if (path)
		strcpy_s(realPath, 64, path);
	else
		strcpy_s(realPath, 64, DEFAULT_MESH_SAVE_PATH);

	sprintf_s(realPath, 64, "%s%s", realPath, file);

	_LOG(LOG_WARN, "Importing file from: '%s'.", realPath);

	char* buffer;
	uint fileSize = app->fs->load(realPath, &buffer);

	if (buffer && fileSize > 0)
	{
		uint ranges[5];
		char* cursor = buffer;
		uint bytes = sizeof(ranges);

		//Ranges
		memcpy(ranges, cursor, bytes);

		_LOG(LOG_WARN, "Num of indices: %d", ranges[0]);
		_LOG(LOG_WARN, "Num of vertices: %d", ranges[1]);
		_LOG(LOG_WARN, "Num of normals: %d", ranges[2]);
		_LOG(LOG_WARN, "Num of texCoords: %d", ranges[3]);
		_LOG(LOG_WARN, "Num of VertexColors: %d", ranges[4]);
		_LOG(LOG_STD, "---------------------------------------");

		//Indices
		cursor += bytes;
		bytes = sizeof(ranges[0]);



		//Vertices
		/*cursor += bytes;
		bytes = sizeof(ranges[1]) * 3;

		//Normals
		cursor += bytes;
		bytes = sizeof(ranges[2]) * 3;

		//TexCoords
		cursor += bytes;
		bytes = sizeof(ranges[3]) * 3;

		//Colors
		cursor += bytes;
		bytes = sizeof(ranges[4]) * 3;*/

	}
	else
	{
		_LOG(LOG_ERROR, "Error while loading %s.", file);
	}

	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(realPath);
}

void ModuleImporter::loadMesh(const char* file, Mesh* mesh)
{
	if (!file || !mesh)
	{
		_LOG(LOG_ERROR, "Error: Invalid file name or mesh component.");
		return;
	}

	char* realPath = new char[64];
	sprintf_s(realPath, 64, "%s%s", DEFAULT_MESH_SAVE_PATH, file);

	_LOG(LOG_WARN, "Importing file from: '%s'.", realPath);

	char* buffer;
	uint fileSize = app->fs->load(realPath, &buffer);

	if (buffer && fileSize > 0)
	{
		uint ranges[5];
		char* cursor = buffer;
		uint bytes = sizeof(ranges);

		//Ranges
		memcpy(ranges, cursor, bytes);

		mesh->numIndices = ranges[0];
		mesh->numVertices = ranges[1];
		mesh->numNormals = ranges[2];
		mesh->numTexCoords = ranges[3];
		//Colors still not in use

		//Indices
		cursor += bytes;
		bytes = sizeof(ranges[0]);

		mesh->indices = new uint[mesh->numIndices];
		memcpy(mesh->indices, cursor, bytes);

		//Vertices
		cursor += bytes;
		bytes = sizeof(ranges[1]) * 3;

		mesh->vertices = new float[mesh->numVertices * 3];
		memcpy(mesh->vertices, cursor, bytes);

		//Normals
		cursor += bytes;
		bytes = sizeof(ranges[2]) * 3;

		mesh->normals = new float[mesh->numNormals * 3];
		memcpy(mesh->normals, cursor, bytes);

		//TexCoords
		cursor += bytes;
		bytes = sizeof(ranges[3]) * 2;

		mesh->texCoords = new float[mesh->numTexCoords * 2];
		memcpy(mesh->texCoords, cursor, bytes);

		//Colors
		cursor += bytes;
		bytes = sizeof(ranges[4]) * 3;

		//Not in use
		//TODO

		mesh->loadToOpenGl();
	}
	else
	{
		_LOG(LOG_ERROR, "Error while loading %s.", file);
	}

	RELEASE_ARRAY(buffer);
	RELEASE_ARRAY(realPath);
}