#include "Globals.h"
#include "Application.h"
#include "ImporterTexture.h"

#include "ResourceTexture.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ImporterTexture::ImporterTexture() : Importer()
{
	_LOG(LOG_STD, "Creating a texture importer.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		_LOG(LOG_ERROR, "Error while Devil Init: %s\n", iluErrorString(devilError));
	}
}


ImporterTexture::~ImporterTexture()
{
	ilShutDown();
}

bool ImporterTexture::import(const char* originalFile, std::string& exportedFile, UID& resUID)
{
	bool ret = false;

	std::string file, ext, origin;
	app->fs->splitPath(originalFile, NULL, &file, &ext);
	origin.assign(DEFAULT_TEXTURES_PATH + file);

	char* buffer = NULL;
	uint size = app->fs->load(origin.c_str(), &buffer);

	if (buffer && size > 0)
		ret = importBuf(buffer, size, exportedFile, resUID);

	RELEASE_ARRAY(buffer);

	if (!ret)
		_LOG(LOG_ERROR, "Could not load texture %s.", originalFile);

	return ret;
}

bool ImporterTexture::importBuf(const void* buffer, uint size, std::string& exportedFile, UID& resUID)
{
	bool ret = false;

	/**First load the image */
	//1-Gen the image and bind it
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	//2-Load the image from buffer
	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint _size;
		ILubyte* data = NULL;

		//3-Set format (DDS, DDS compression)
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		//4-Get size
		_size = ilSaveL(IL_DDS, NULL, 0);
		if (_size > 0)
		{
			//5-If size is more than 0 create the image buffer
			data = new ILubyte[_size];

			//6-Save the image with the correct format and save it with file system
			if (ilSaveL(IL_DDS, data, _size) > 0)
			{
				//Save it FS
				resUID = app->resourceManager->getNewUID();
				exportedFile.assign(std::to_string(resUID) + TEXTURE_EXTENSION);
				char savePath[256];
				sprintf_s(savePath, 256, "%s%s", DEFAULT_TEXTURE_SAVE_PATH, exportedFile.c_str());

				if (app->fs->save(savePath, (const char*)data, _size) == _size)
					ret = true;
				else
					_LOG(LOG_ERROR, "Error importing texture.");
			}

			//7-Release the image buffer to avoid memory leaks
			RELEASE_ARRAY(data);
		}

		//8-Finally if the image was loaded destroy the image to avoid more memory leaks
		ilDeleteImages(1, &image);
	}

	return ret;
}

/*void ImporterTexture::importTexture(const char* fileName, ResourceTexture* resTex) //Note in texture case the output name will be a new UID + extension
{//TODO: outputName should be the original file with all the path? The new file? Which extension should have the outputName
	if (!fileName || !resTex)
	{
		_LOG(LOG_ERROR, "Error loading texture: invalid file name.");
		return;
	}

	//TODO: Assign the path directly here? Assume file name is clear?... For now the fileName is clear and the path is added here
	std::string outputName(DEFAULT_TEXTURES_PATH);
	outputName.append(fileName);

	resTex->originalFile.assign(outputName);

	_LOG(LOG_INFO, "Loading a new texture: %s.", outputName.c_str());

	char* buffer;
	uint size = app->fs->load(outputName.c_str(), &buffer);

	if (buffer && size > 0)
	{
		//First load the image with unknown format //TODO: maybe identify first the file format and load the file with the correct format
		//For now will use UNKNOWN parameter
		//1-Gen the image and bind it
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		//TODO:
		/*ILenum format;
		switch (texFormat)
		{
		default: format = IL_TYPE_UNKNOWN;
			break;
		}*/

		//2-Load the image from buffer
		/*if (ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
		{
			//Check overwitting parameter (ilEnable). if textuer already exist maybe shoudl overwrite it??
			
			ILubyte* data;
			ILuint size;
			//3-Set format (DDS, DDS compression)
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			//4-Get size
			size = ilSaveL(IL_DDS, NULL, 0);
			if ( size > 0)
			{
				//5-If size is more than 0 create the image buffer
				data = new ILubyte[size];

				//6-Save the image with the correct format and save it with file system
				if (ilSaveL(IL_DDS, data, size) > 0)
				{
					uint tmpId = app->resourceManager->getNewUID();
					std::string savePath(DEFAULT_TEXTURE_SAVE_PATH + std::to_string(tmpId) + TEXTURE_EXTENSION);
					if (app->fs->save(savePath.c_str(), (const char*)data, size) != size)
					{
						_LOG(LOG_ERROR, "ERROR saving the dds texture.");
					}
					else
					{
						_LOG(LOG_INFO, "New dds created: %s.", savePath.c_str());

						//------
						char exported[64];
						strcpy_s(exported, 64, fileName);
						//Clean the extension--------------
						uint s = strlen(fileName);
						if (s > 0)
						{
							char* it = exported;
							it += s;

							while (*it != '.')
							{
								--it;
								--s;
							}

							if (s < strlen(exported))
								exported[s] = '\0';

							strcat_s(exported, 64, ".dds");
						}
						//------//NOTE: This is really cool but realized that resTex->exportedFile should be the same us save path but without full path

						resTex->exportedFile.assign(std::to_string(tmpId) + TEXTURE_EXTENSION);
					}
				}

				//7-Release the image buffer to avoid memory leaks
				RELEASE_ARRAY(data);
			}

			//8-Finally if the image was loaded destroy the image to avoid more memory leaks
			ilDeleteImages(1, &image);
		}
		else
			_LOG(LOG_ERROR, "Could not load the image: %s.", outputName.c_str());
		
	}
	else
		_LOG(LOG_ERROR, "Could not load the image: %s.", outputName.c_str());

	RELEASE_ARRAY(buffer);
}*/

bool ImporterTexture::loadTexture(ResourceTexture* resTex)
{
	bool ret = false;

	if (!resTex)
	{
		_LOG(LOG_ERROR, "Error loading a texture, invalid texture resource.");
		return ret;
	}

	if (resTex->exportedFile.empty())
	{
		_LOG(LOG_ERROR, "Error loading a texture .dds: Invalid file name or texture resource.");
		return ret;
	}

	//Assume fileName is clear
	std::string realPath; //TODO: Dont use std

	realPath.assign(DEFAULT_TEXTURE_SAVE_PATH);

	realPath.append(resTex->exportedFile.c_str());

	_LOG(LOG_INFO, "Loading texture: %s.", realPath.c_str());

	//TODO: Actually load the texture

	char* buffer = NULL; //TODO: Would be a good practise to set to NULL all buffer char pointers
	uint size = app->fs->load(realPath.c_str(), &buffer);

	if (buffer && size > 0)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoadL(IL_DDS, (const void*)buffer, size))
		{
			resTex->textureGlID = ilutGLBindTexImage();
			ilDeleteImages(1, &image);
			ret = true;

			resTex->addInstance();
			app->resourceManager->addResource(resTex, resTex->getUID());
		}
	}
	else
		_LOG(LOG_ERROR, "Error loading dds: %s.", resTex->exportedFile.c_str());

	RELEASE_ARRAY(buffer);

	return ret;
}


